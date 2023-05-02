/*
 * @Author: gongluck
 * @Date: 2023-05-02 23:16:58
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-03 00:49:14
 */

// clang -target bpf -g -O2 -D __TARGET_ARCH_x86 -c uprobe.bpf.c -o uprobe.bpf.o
// bpftool gen skeleton uprobe.bpf.o > uprobe.skel.h

#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("uprobe")
int BPF_KPROBE(uprobe_add, int a, int b)
{
    bpf_printk("uprobed_add ENTRY: a = %d, b = %d", a, b);
    return 0;
}

SEC("uretprobe")
int BPF_KRETPROBE(uretprobe_add, int ret)
{
    bpf_printk("uprobed_add EXIT: return = %d", ret);
    return 0;
}

SEC("uprobe//proc/self/exe:uprobed_sub")
int BPF_KPROBE(uprobe_sub, int a, int b)
{
    bpf_printk("uprobed_sub ENTRY: a = %d, b = %d", a, b);
    return 0;
}

SEC("uretprobe//proc/self/exe:uprobed_sub")
int BPF_KRETPROBE(uretprobe_sub, int ret)
{
    bpf_printk("uprobed_sub EXIT: return = %d", ret);
    return 0;
}