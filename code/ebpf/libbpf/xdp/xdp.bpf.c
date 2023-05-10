/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:35:21
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-08 11:39:52
 */

/*
# bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
clang -target bpf -g -O2 -D __TARGET_ARCH_x86 -I /usr/include/x86_64-linux-gnu/ -c xdp.bpf.c -o xdp.bpf.o
# bpftool gen skeleton xdp.bpf.o > xdp.skel.h
*/

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include "xdp.struct.h"

struct bpf_map_def
{
	__uint(type, MAPTYPE);
	__uint(key_size, sizeof(__u32));
	__uint(value_size, sizeof(struct xdp_struct));
	__uint(max_entries, XDP_REDIRECT + 1);
} xdp_stats_map SEC(".maps");

static __always_inline int parse_eth(struct ethhdr *eth)
{
	switch (bpf_htons(eth->h_proto))
	{
	case ETH_P_IP:
		// bpf_printk("eth type : ETH_P_IP\n");
		break;
	case ETH_P_IPV6:
		// bpf_printk("eth type : ETH_P_IPV6\n");
		break;
	default:
		bpf_printk("eth type : 0x%04x\n", bpf_htons(eth->h_proto));
		break;
	}
	return eth->h_proto;
}

static __always_inline int parse_ipv4(struct iphdr *ip)
{
	switch (ip->protocol)
	{
	case IPPROTO_TCP:
		// bpf_printk("ip type : IPPROTO_TCP\n");
		break;
	case IPPROTO_UDP:
		// bpf_printk("ip type : IPPROTO_UDP\n");
		break;
	default:
		bpf_printk("ip type : 0x%04x\n", ip->protocol);
		break;
	}
	return ip->protocol;
}

SEC("xdp")
int xdp_pass_func(struct xdp_md *ctx)
{
	// bpf_printk("pass");

	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	int size = ctx->data_end - ctx->data;
	int protocol = -1;
	int hdrsize = 0;

	// parse packet
	struct ethhdr *eth = data;
	hdrsize = sizeof(*eth);
	if ((void *)(eth + 1) > data_end)
	{
		bpf_printk("not eth");
	}
	else
	{
		protocol = parse_eth(eth);
		if (bpf_htons(protocol) == ETH_P_IP)
		{
			struct iphdr *ipv4 = eth + 1;
			if ((void *)(ipv4 + 1) > data_end)
			{
				bpf_printk("not ipv4");
			}
			else
			{
				hdrsize = ipv4->ihl /*4位首部长度*/ * 4;
				if (hdrsize < sizeof(*ipv4))
				{
					bpf_printk("ipv4 size error %d < %d\n", hdrsize, sizeof(*ipv4));
				}
				else
				{
					protocol = parse_ipv4(ipv4);
					if (protocol == IPPROTO_TCP)
					{
						struct tcphdr *tcp = ipv4 + 1;
						if ((void *)(tcp + 1) > data_end)
						{
							bpf_printk("not tcp");
						}
						else
						{
							hdrsize = tcp->doff /*头偏移，32位单位*/ * 4;
							if (hdrsize < sizeof(*tcp))
							{
								bpf_printk("tcp size error %d < %d\n", hdrsize, sizeof(*tcp));
							}
							else
							{
								// bpf_printk("tcp all size : %ld\n", size);
							}
						}
					}
					else if (protocol == IPPROTO_UDP)
					{
						struct udphdr *udp = ipv4 + 1;
						if ((void *)(udp + 1) > data_end)
						{
							bpf_printk("not udp");
						}
						else
						{
							hdrsize = sizeof(*udp);
							if (hdrsize > bpf_ntohs(udp->len))
							{
								bpf_printk("udp size error %d < %d\n", bpf_ntohs(udp->len), sizeof(*udp));
							}
							else
							{
								bpf_printk("udp data size : %ld\n", bpf_ntohs(udp->len));
							}
						}
					}
				}
			}
		}
	}

	__u32 key = XDP_PASS;
	struct xdp_struct *value = bpf_map_lookup_elem(&xdp_stats_map, &key);
	if (value == NULL)
	{
		return XDP_ABORTED;
	}
	if (MAPTYPE == BPF_MAP_TYPE_ARRAY)
	{
		__sync_fetch_and_add(&value->rx_packets, 1);
		__sync_fetch_and_add(&value->rx_bytes, size);
	}
	else if (MAPTYPE == BPF_MAP_TYPE_PERCPU_ARRAY)
	{
		value->rx_packets += 1;
		value->rx_bytes += size;
	}

	return XDP_PASS;
}

SEC("xdp")
int xdp_drop_func(struct xdp_md *ctx)
{
	bpf_printk("drop");
	return XDP_DROP;
}

SEC("xdp")
int xdp_aborted_func(struct xdp_md *ctx)
{
	bpf_printk("aborted");
	return XDP_ABORTED;
}

char _license[] SEC("license") = "GPL";