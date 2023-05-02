/*
 * @Author: gongluck
 * @Date: 2023-05-02 20:35:23
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-02 22:43:35
 */

// bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
// clang -target bpf -g -O2 -c bootstrap.bpf.c -o bootstrap.bpf.o
// bpftool gen skeleton bootstrap.bpf.o > bootstrap.skel.h

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include "bootstrap.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 8192);
    __type(key, pid_t);
    __type(value, u64);
} start_time SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} event_ringbuf SEC(".maps");

// cat /sys/kernel/debug/tracing/events/sched/sched_process_exec/format
// bpftrace -lv tracepoint:sched:sched_process_exec
SEC("tracepoint/sched/sched_process_exec")
int handle_exec(struct trace_event_raw_sched_process_exec *ctx)
{
    u64 id = bpf_get_current_pid_tgid();
    pid_t pid = id >> 32;
    u64 ts = bpf_ktime_get_ns();
    bpf_map_update_elem(&start_time, &pid, &ts, BPF_ANY);

    struct myevent *myevent = bpf_ringbuf_reserve(&event_ringbuf, sizeof(*myevent), 0);
    if (myevent == NULL)
        return 0;

    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    myevent->exit_event = false;
    myevent->pid = pid;
    myevent->tgid = BPF_CORE_READ(task, real_parent, tgid);
    bpf_get_current_comm(&myevent->comm, sizeof(myevent->comm));

    unsigned fname_off = ctx->__data_loc_filename & 0xFFFF;
    bpf_probe_read_str(&myevent->filename, sizeof(myevent->filename), (void *)ctx + fname_off);

    bpf_ringbuf_submit(myevent, 0);
    return 0;
}

SEC("tracepoint/sched/sched_process_exit")
int handle_exit(struct trace_event_raw_sched_process_template *ctx)
{
    u64 id = bpf_get_current_pid_tgid();
    pid_t pid = id >> 32;

    u64 duration_ns = -1;
    u64 *start_ts = bpf_map_lookup_elem(&start_time, &pid);
    if (start_ts != NULL)
    {
        duration_ns = bpf_ktime_get_ns() - *start_ts;
    }
    bpf_map_delete_elem(&start_time, &pid);
    if (start_ts == NULL || duration_ns < 1000000)
    {
        return 0;
    }

    struct myevent *myevent = bpf_ringbuf_reserve(&event_ringbuf, sizeof(*myevent), 0);
    if (!myevent)
        return 0;

    struct task_struct *task = (struct task_struct *)bpf_get_current_task();

    myevent->exit_event = true;
    myevent->duration_ns = duration_ns;
    myevent->pid = pid;
    myevent->tgid = BPF_CORE_READ(task, real_parent, tgid);
    myevent->exit_code = BPF_CORE_READ(task, exit_code);
    bpf_get_current_comm(&myevent->comm, sizeof(myevent->comm));

    bpf_ringbuf_submit(myevent, 0);
    return 0;
}
