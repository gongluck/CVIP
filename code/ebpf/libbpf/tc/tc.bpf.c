/*
 * @Author: gongluck
 * @Date: 2023-05-12 15:00:54
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-12 16:05:39
 */

/*
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
clang -target bpf -g -O2 -D __TARGET_ARCH_x86 -I /usr/include/x86_64-linux-gnu/ -c tc.bpf.c -o tc.bpf.o
bpftool gen skeleton tc.bpf.o> tc.skel.h
*/

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#ifndef ETH_P_IP        // #include <linux/if_ether.h>
#define ETH_P_IP 0x0800 /* Internet Protocol packet	*/
#endif

#ifndef TC_ACT_OK // #include <linux/pkt_cls.h>
#define TC_ACT_OK 0
#endif

SEC("tc")
int tc_ingress(struct __sk_buff *ctx)
{
    void *data_end = (void *)(__u64)ctx->data_end;
    void *data = (void *)(__u64)ctx->data;
    struct ethhdr *l2;
    struct iphdr *l3;

    if (ctx->protocol != bpf_htons(ETH_P_IP))
        return TC_ACT_OK;

    l2 = data;
    if ((void *)(l2 + 1) > data_end)
        return TC_ACT_OK;

    l3 = (struct iphdr *)(l2 + 1);
    if ((void *)(l3 + 1) > data_end)
        return TC_ACT_OK;

    bpf_printk("Got IP packet: tot_len: %d, ttl: %d", bpf_ntohs(l3->tot_len), l3->ttl);
    return TC_ACT_OK;
}

char __license[] SEC("license") = "GPL";