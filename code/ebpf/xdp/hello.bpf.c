/*
 * @Author: gongluck
 * @Date: 2023-04-27 10:46:50
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-27 11:51:26
 */

// clang -target bpf -I /usr/include/$(uname -m)-linux-gnu -g -O2 -c hello.bpf.c -o hello.bpf.o
// bpftool prog load hello.bpf.o /sys/fs/bpf/hello
// bpftool prog list --pretty
// bpftool prog show id [id] --pretty
// bpftool prog dump xlated id [id]
// bpftool prog dump jited id [id]
// bpftool net attach xdp id [id] dev lo
// bpftool net list
// ip link
// cat /sys/kernel/debug/tracing/trace_pipe
// bpftool prog tracelog
// bpftool map list
// bpftool map dump id [id]
// bpftool net detach xdp dev lo
// rm /sys/fs/bpf/hello

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

int counter = 0;

SEC("xdp")
int hello(struct xdp_md *ctx)
{
    bpf_printk("Hello World %d", counter);
    counter++;
    return XDP_PASS;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";