/*
 * @Author: gongluck
 * @Date: 2023-04-30 22:52:55
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-30 23:28:27
 */

// clang -target bpf -g -O2 -c minimal.bpf.c -o minimal.bpf.o
// bpftool gen skeleton minimal.bpf.o > minimal.skel.h

// #define BPF_NO_GLOBAL_DATA
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// 旧内核可能不支持全局变量
int my_pid = 0;

// map映射
struct
{
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1);
    __type(key, int);
    __type(value, int);
} my_pid_map SEC(".maps");

// cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_write/format
// bpftrace -lv tracepoint:syscalls:sys_enter_write
SEC("tracepoint/syscalls/sys_enter_write")
int handle_tracepoint()
{
    int index = 0;
    int pid = bpf_get_current_pid_tgid() >> 32;
    int *my_pid = bpf_map_lookup_elem(&my_pid_map, &index);

    if (!my_pid || *my_pid != pid)
        return 1;

    bpf_printk("BPF triggered from PID %d.\n", pid);

    return 0;
}