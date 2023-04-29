/*
 * @Author: gongluck
 * @Date: 2023-04-29 17:52:03
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-29 18:00:40
 */

// clang -target bpf -I /usr/include/$(uname -m)-linux-gnu -g -O2 -D __TARGET_ARCH_x86 -c hello_buffer_config.bpf.c -o hello_buffer_config.bpf.o
// bpftool gen skeleton hello_buffer_config.bpf.o > hello_buffer_config.skel.h

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include "hello_buffer_config.h"

char message[12] = "Hello World";

struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
} output SEC(".maps");

struct user_msg_t
{
    char message[12];
};

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, u32);
    __type(value, struct user_msg_t);
} my_config SEC(".maps");

SEC("kprobe/sys_exec")
int BPF_KPROBE(hello, const char *pathname)
{
    struct data_t data = {};
    struct user_msg_t *p;

    data.pid = bpf_get_current_pid_tgid() >> 32;
    data.uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;

    bpf_get_current_comm(&data.command, sizeof(data.command));
    bpf_probe_read_user_str(&data.path, sizeof(data.path), pathname);

    p = bpf_map_lookup_elem(&my_config, &data.uid);
    if (p != 0)
    {
        bpf_probe_read_kernel_str(&data.message, sizeof(data.message), p->message);
    }
    else
    {
        bpf_probe_read_kernel_str(&data.message, sizeof(data.message), message);
    }

    bpf_perf_event_output(ctx, &output, BPF_F_CURRENT_CPU, &data, sizeof(data));
    return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";