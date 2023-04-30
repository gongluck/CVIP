/*
 * @Author: gongluck
 * @Date: 2023-04-30 22:52:55
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-30 23:28:27
 */

// clang -target bpf -g -O2 -c minimal.bpf.c -o minimal.bpf.o
// bpftool gen skeleton minimal.bpf.o > minimal.skel.h

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

int my_pid = 0;

// cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_write/format
// bpftrace -lv tracepoint:syscalls:sys_enter_write
SEC("tracepoint/syscalls/sys_enter_write")
int handle_tracepoint()
{
    int pid = bpf_get_current_pid_tgid() >> 32;

    if (pid != my_pid)
        return 0;

    bpf_printk("BPF triggered from PID %d.\n", pid);

    return 0;
}