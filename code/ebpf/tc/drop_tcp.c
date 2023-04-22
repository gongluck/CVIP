/*
 * @Author: gongluck
 * @Date: 2023-04-22 20:28:21
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-22 21:45:13
 */

#include <stdbool.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/pkt_cls.h>
#include <stdio.h>

#include "bpf_endian.h"
#include "bpf_helpers.h"

typedef unsigned int u32;
#define bpfprint(fmt, ...)                         \
    ({                                             \
        char ____fmt[] = fmt;                      \
        bpf_trace_printk(____fmt, sizeof(____fmt), \
                         ##__VA_ARGS__);           \
    })

/*
  check whether the packet is of TCP protocol
*/
static __inline bool is_TCP(void *data_begin, void *data_end)
{
    bpfprint("Entering is_TCP\n");
    struct ethhdr *eth = data_begin;

    // Check packet's size
    // the pointer arithmetic is based on the size of data type, current_address plus int(1) means:
    // new_address= current_address + size_of(data type)
    if ((void *)(eth + 1) > data_end) //
        return false;

    // Check if Ethernet frame has IP packet
    if (eth->h_proto == bpf_htons(ETH_P_IP))
    {
        struct iphdr *iph = (struct iphdr *)(eth + 1); // or (struct iphdr *)( ((void*)eth) + ETH_HLEN );
        if ((void *)(iph + 1) > data_end)
            return false;

        // extract src ip and destination ip
        u32 ip_src = iph->saddr;
        u32 ip_dst = iph->daddr;

        //
        bpfprint("src ip addr1: %d.%d.%d\n", (ip_src)&0xFF, (ip_src >> 8) & 0xFF, (ip_src >> 16) & 0xFF);
        bpfprint("src ip addr2:.%d\n", (ip_src >> 24) & 0xFF);

        bpfprint("dest ip addr1: %d.%d.%d\n", (ip_dst)&0xFF, (ip_dst >> 8) & 0xFF, (ip_dst >> 16) & 0xFF);
        bpfprint("dest ip addr2: .%d\n", (ip_dst >> 24) & 0xFF);

        // Check if IP packet contains a TCP segment
        if (iph->protocol == IPPROTO_TCP)
            return true;
    }
    return false;
}

// SEC("xdp")
// int xdp_drop_tcp(struct xdp_md *ctx)
// {
//     void *data_end = (void *)(long)ctx->data_end;
//     void *data = (void *)(long)ctx->data;

//     if (is_TCP(data, data_end))
//         return XDP_DROP;

//     return XDP_PASS;
// }

SEC("tc")
int tc_drop_tcp(struct __sk_buff *skb)
{

    bpfprint("Entering tc section\n");
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    if (is_TCP(data, data_end))
        return TC_ACT_SHOT;
    else
        return TC_ACT_OK;
}

char _license[] SEC("license") = "GPL";