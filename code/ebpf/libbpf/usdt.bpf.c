/*
 * @Author: gongluck
 * @Date: 2023-05-03 01:44:56
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-03 22:06:09
 */

/*
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
clang -target bpf -g -O2 -D __TARGET_ARCH_x86 -I /usr/include/x86_64-linux-gnu/ -c usdt.bpf.c -o usdt.bpf.o
bpftool gen object usdt.bpf.obj.o usdt.bpf.o
bpftool gen skeleton usdt.bpf.obj.o > usdt.skel.h
*/

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/usdt.bpf.h>

pid_t my_pid;

// https://www.gnu.org/software/libc/manual/html_node/Non_002dlocal-Goto-Probes.html
SEC("usdt/libc.so.6:libc:setjmp")
int BPF_USDT(usdt_auto_attach, void *arg1, int arg2, void *arg3)
{
    pid_t pid = bpf_get_current_pid_tgid() >> 32;
    if (pid != my_pid)
    {
        return 0;
    }
    bpf_printk("USDT auto attach to libc:setjmp: arg1 = %lx, arg2 = %d, arg3 = %lx", arg1, arg2, arg3);
    return 0;
}

SEC("usdt")
int BPF_USDT(usdt_manual_attach, void *arg1, int arg2, void *arg3)
{
    bpf_printk("USDT manual attach to libc:setjmp: arg1 = %lx, arg2 = %d, arg3 = %lx", arg1, arg2, arg3);
    return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";