/*
 * @Author: gongluck
 * @Date: 2023-04-27 11:55:21
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-27 15:47:43
 */

// clang -target bpf -I /usr/include/$(uname -m)-linux-gnu -g -O2 -c hello_func.bpf.c -o hello_func.bpf.o
// bpftool prog load hello_func.bpf.o /sys/fs/bpf/hello type raw_tp

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

static __attribute((noinline)) int get_opcode(struct bpf_raw_tracepoint_args *ctx)
{
    return ctx->args[1];
}

SEC("raw_tp")
int hello(struct bpf_raw_tracepoint_args *ctx)
{
    int opcode = get_opcode(ctx);
    bpf_printk("Syscall: %d", opcode);
    return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";