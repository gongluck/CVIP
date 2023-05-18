/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:35:21
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-18 15:03:56
 */

/*
clang -target bpf -g -O2 -D __x86_64__ -D __TARGET_ARCH_x86 -I /usr/include/x86_64-linux-gnu/ -c xdp.bpf.c -o xdp.bpf.o
bpftool gen skeleton xdp.bpf.o > xdp.skel.h
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
#include <linux/icmp.h>

#include "xdp.struct.h"

// extern
unsigned char g_gataway_mac[ETH_ALEN] = {0xfa, 0x16, 0x3e, 0xff, 0x8b, 0xd5}; // route -n && arp 192.168.0.1
unsigned char g_local_mac[ETH_ALEN] = {0xfa, 0x16, 0x3e, 0xb0, 0x08, 0xb5};	  // ifconfig
__u32 g_icmp_server_ip = bpf_htonl(0x0e7768fe);								  // baidu 14.119.104.189
__u32 g_tcp_server_ip = bpf_htonl(0x0e7768fe);
__u32 g_udp_server_ip = bpf_htonl(0xdb904f71);
__u32 g_local_ip = bpf_htonl(0xc0a800ad); // 192.168.0.173
int g_show_trace = 0;

#define showtrace(fmt, args...)                    \
	do                                             \
	{                                              \
		if (g_show_trace != 0)                     \
			___bpf_pick_printk(args)(fmt, ##args); \
	} while (0)

// 数据统计
struct xdp_stats_map
{
	__uint(type, MAPTYPE);
	__uint(key_size, sizeof(__u32));
	__uint(value_size, sizeof(struct xdp_struct));
	__uint(max_entries, XDP_REDIRECT + 1);
} xdp_stats_map SEC(".maps");

// 客户端信息
struct xdp_client_map
{
	__uint(type, BPF_MAP_TYPE_LRU_HASH);
	__type(key, __u32);
	__type(value, __u32);
	__uint(max_entries, 65535);
} xdp_client_map SEC(".maps");

/*											check packet data avaliable												*/

#define check_header_fix(headertype, data, data_end, offset, pvalue)                           \
	BPFCHECKGOTO(((data + offset) + sizeof(headertype)) < data_end, define_check_udp_fix_out); \
	pvalue = data + offset;                                                                    \
	define_check_udp_fix_out:

// for ehthdr
static __always_inline struct ethhdr *check_ethhdr_fix(void *data, void *data_end, unsigned int offset)
{
	struct ethhdr *eth = NULL;
	check_header_fix(struct ethhdr, data, data_end, offset, eth);
	return eth;
}

// for iphdr
static __always_inline struct iphdr *check_ipv4_fix(void *data, void *data_end, unsigned int offset)
{
	struct iphdr *ipv4 = NULL;
	check_header_fix(struct iphdr, data, data_end, offset, ipv4);
	return ipv4;
}

// for icmphdr
static __always_inline struct icmphdr *check_icmp_fix(void *data, void *data_end, unsigned int offset)
{
	struct icmphdr *icmp = NULL;
	check_header_fix(struct icmphdr, data, data_end, offset, icmp);
	return icmp;
}

// for tcphdr
static __always_inline struct tcphdr *check_tcp_fix(void *data, void *data_end, unsigned int offset)
{
	struct tcphdr *tcp = NULL;
	check_header_fix(struct tcphdr, data, data_end, offset, tcp);
	return tcp;
}

// for udphdr
static __always_inline struct udphdr *check_udp_fix(void *data, void *data_end, unsigned int offset)
{
	struct udphdr *udp = NULL;
	check_header_fix(struct udphdr, data, data_end, offset, udp);
	return udp;
}

// dump data
static __always_inline int dump_data(void *data, unsigned int len)
{
	for (unsigned int i = 0; i < len; ++i)
	{
		bpf_printk("0x%02x", ((unsigned char *)data)[i]);
	}
	return 0;
}

/*											checksum											*/
static __always_inline __u32 make_checksum_from(__u32 n)
{
	return (n >> 16) + (n & 0xFFFF);
}
static __always_inline __u16 make_checksum_from32(__u32 n)
{
	return make_checksum_from(make_checksum_from(n));
}

// create icmp echo reply
static __always_inline int icmp_reply(struct ethhdr *eth, struct iphdr *ipv4, struct icmphdr *icmp)
{
	// swap mac
	SWAPDATA(eth->h_source, eth->h_dest, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0;
	struct iphdr ipv4_old = *ipv4;

	// swap ip
	SWAPDATA(ipv4->saddr, ipv4->daddr, __be32, 1);

	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	old_csum = icmp->checksum;
	icmp->checksum = 0; // 在计算校验和时，需要将 ICMP 头部中的 checksum 字段置为 0，以免影响校验和的计算。
	struct icmphdr icmp_old = *icmp;
	icmp->type = ICMP_ECHOREPLY; // 修改icmp的type

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	csum = bpf_csum_diff((__be32 *)&icmp_old, sizeof(icmp_old), (__be32 *)icmp, sizeof(*icmp), ~old_csum /*取反操作还原成原累加数*/);
	icmp->checksum = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int icmp_request_server(struct ethhdr *eth, struct iphdr *ipv4, struct icmphdr *icmp)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);		// dst(myself) -> src
	COPYDATA(ipv4->daddr, g_icmp_server_ip, __be32, 1); // baidu -> dst

	// ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int icmp_reply_server(struct ethhdr *eth, struct iphdr *ipv4, __u32 client_addr)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);  // dst(myself) -> src
	COPYDATA(ipv4->daddr, client_addr, __be32, 1); // baidu -> dst

	// ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int tcp_redirect_to_server(struct ethhdr *eth, struct iphdr *ipv4, struct tcphdr *tcp)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// old pseudo header
	struct pseudo_hdr pseudo_hdr_old = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, bpf_ntohs(ipv4->tot_len) - ipv4->ihl * 4 - sizeof(*tcp)};

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);
	COPYDATA(ipv4->daddr, g_tcp_server_ip, __be32, 1);

	ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	// udp checksum
	struct pseudo_hdr pseudo_hdr = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, bpf_ntohs(ipv4->tot_len) - ipv4->ihl * 4 - sizeof(*tcp)};
	csum = bpf_csum_diff((__be32 *)&pseudo_hdr_old, sizeof(pseudo_hdr_old), (__be32 *)&pseudo_hdr, sizeof(pseudo_hdr), ~(tcp->check) /*取反操作还原成原累加数*/);
	tcp->check = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int tcp_redirect_to_client(struct ethhdr *eth, struct iphdr *ipv4, struct tcphdr *tcp, __u32 client_addr)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// old pseudo header
	struct pseudo_hdr pseudo_hdr_old = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, bpf_ntohs(ipv4->tot_len) - ipv4->ihl * 4 - sizeof(*tcp)};

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);
	COPYDATA(ipv4->daddr, client_addr, __be32, 1);

	ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	// udp checksum
	struct pseudo_hdr pseudo_hdr = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, bpf_ntohs(ipv4->tot_len) - ipv4->ihl * 4 - sizeof(*tcp)};
	csum = bpf_csum_diff((__be32 *)&pseudo_hdr_old, sizeof(pseudo_hdr_old), (__be32 *)&pseudo_hdr, sizeof(pseudo_hdr), ~(tcp->check) /*取反操作还原成原累加数*/);
	tcp->check = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int udp_redirect_to_server(struct ethhdr *eth, struct iphdr *ipv4, struct udphdr *udp)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// old pseudo header
	struct pseudo_hdr pseudo_hdr_old = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, udp->len};

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);
	COPYDATA(ipv4->daddr, g_udp_server_ip, __be32, 1);

	ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	// udp checksum
	struct pseudo_hdr pseudo_hdr = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, udp->len};
	csum = bpf_csum_diff((__be32 *)&pseudo_hdr_old, sizeof(pseudo_hdr_old), (__be32 *)&pseudo_hdr, sizeof(pseudo_hdr), ~(udp->check) /*取反操作还原成原累加数*/);
	udp->check = ~make_checksum_from32(csum);

	return 0;
}

static __always_inline int udp_redirect_to_client(struct ethhdr *eth, struct iphdr *ipv4, struct udphdr *udp, __u32 client_addr)
{
	// swap mac
	COPYDATA(eth->h_source, g_local_mac, unsigned char, ETH_ALEN);
	COPYDATA(eth->h_dest, g_gataway_mac, unsigned char, ETH_ALEN);

	__sum16 old_csum = ipv4->check;
	ipv4->check = 0; // 在计算校验和时，需要将 IP 头部中的 check 字段置为 0，以免影响校验和的计算。
	struct iphdr ipv4_old = *ipv4;

	// old pseudo header
	struct pseudo_hdr pseudo_hdr_old = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, udp->len};

	// swap ip
	COPYDATA(ipv4->saddr, g_local_ip, __be32, 1);
	COPYDATA(ipv4->daddr, client_addr, __be32, 1);

	ipv4->ttl -= 1;

	// csum = (diff = newsum - oldsum) + oldsum = newsum
	__u32 csum = bpf_csum_diff((__be32 *)&ipv4_old, sizeof(ipv4_old), (__be32 *)ipv4, sizeof(*ipv4), ~old_csum /*取反操作还原成原累加数*/);
	ipv4->check = ~make_checksum_from32(csum);

	// udp checksum
	struct pseudo_hdr pseudo_hdr = {ipv4->saddr, ipv4->daddr, 0, ipv4->protocol, udp->len};
	csum = bpf_csum_diff((__be32 *)&pseudo_hdr_old, sizeof(pseudo_hdr_old), (__be32 *)&pseudo_hdr, sizeof(pseudo_hdr), ~(udp->check) /*取反操作还原成原累加数*/);
	udp->check = ~make_checksum_from32(csum);

	return 0;
}

SEC("xdp")
int xdp_pass_func(struct xdp_md *ctx)
{
	// bpf_printk("pass");

	int ret = 0;

	int action = XDP_PASS;
	struct xdp_struct *value = NULL;

	__u32 client_identify = 0;
	__u32 *client_addr = 0;

	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	__u64 size = data_end - data;

	// parse packet

	do
	{
		struct ethhdr *eth = check_ethhdr_fix(data, data_end, 0);
		BPFCHECKGOTO((eth != NULL), out);

		switch (bpf_ntohs(eth->h_proto))
		{
		case ETH_P_IP:
		{
			struct iphdr *ipv4 = check_ipv4_fix(data, data_end, sizeof(*eth));
			BPFCHECKGOTO((ipv4 != NULL), out);
			switch (ipv4->protocol)
			{
			case IPPROTO_TCP:
			{
				struct udphdr *tcp = check_tcp_fix(data, data_end, sizeof(*eth) + ipv4->ihl * 4);
				BPFCHECKGOTO((tcp != NULL), out);

				static __u32 client_addr = 0;
				static __u16 client_port = 0;
				if (tcp->dest == bpf_htons(80))
				{
					client_addr = ipv4->saddr;
					client_port = tcp->source;

					tcp_redirect_to_server(eth, ipv4, tcp);

					action = XDP_TX;
				}
				else if (tcp->dest == client_port)
				{
					tcp_redirect_to_client(eth, ipv4, tcp, client_addr);

					action = XDP_TX;
				}
			}
			break;
			case IPPROTO_UDP:
			{
				struct udphdr *udp = check_udp_fix(data, data_end, sizeof(*eth) + ipv4->ihl * 4);
				BPFCHECKGOTO((udp != NULL), out);

				static __u32 client_addr = 0;
				static __u16 client_port = 0;
				if (udp->dest == bpf_htons(38447))
				{
					client_addr = ipv4->saddr;
					client_port = udp->source;

					udp_redirect_to_server(eth, ipv4, udp);

					action = XDP_TX;
				}
				else if (udp->dest == client_port)
				{
					udp_redirect_to_client(eth, ipv4, udp, client_addr);

					action = XDP_TX;
				}
			}
			break;
			case IPPROTO_ICMP:
			{
				struct icmphdr *icmp = check_icmp_fix(data, data_end, sizeof(*eth) + ipv4->ihl * 4);
				BPFCHECKGOTO((icmp != NULL), out);
				// bpf_printk("data :");
				// dump_data(eth, sizeof(struct ethhdr));
				// dump_data(ipv4, sizeof(struct iphdr));
				// dump_data(icmp, sizeof(struct icmphdr));
				switch (icmp->type)
				{
				case ICMP_ECHO:
				{
					showtrace("icmp echo id : %d, seq : %d", bpf_ntohs(icmp->un.echo.id), bpf_ntohs(icmp->un.echo.sequence));

					// icmp_reply(eth, ipv4, icmp);

					COPYDATA(client_identify, icmp->un.reserved, __u8, sizeof(client_identify));
					showtrace("identify : 0x%04x", client_identify);
					showtrace("client_addr : 0x%04x", ipv4->saddr);
					ret = bpf_map_update_elem(&xdp_client_map, &client_identify, &ipv4->saddr, BPF_ANY);
					BPFCHECKGOTO((ret == 0), out);

					icmp_request_server(eth, ipv4, icmp);

					action = XDP_TX;
				}
				break;
				case ICMP_ECHOREPLY:
				{
					showtrace("icmp echoreply id : %d, seq : %d", bpf_ntohs(icmp->un.echo.id), bpf_ntohs(icmp->un.echo.sequence));

					COPYDATA(client_identify, icmp->un.reserved, __u8, sizeof(client_identify));
					showtrace("identify : 0x%04x", client_identify);
					client_addr = bpf_map_lookup_elem(&xdp_client_map, &client_identify);
					if (client_addr == NULL)
					{
						bpf_printk("identify(%d-%d) : 0x%04x not found\n", bpf_ntohs(icmp->un.echo.id), bpf_ntohs(icmp->un.echo.sequence), client_identify);
					}
					BPFCHECKGOTO((client_addr != NULL), out);
					showtrace("client_addr : 0x%04x", *client_addr);

					icmp_reply_server(eth, ipv4, *client_addr);

					action = XDP_TX;
				}
				break;
				default:
					showtrace("unhandle icmp type : 0x%02x\n", icmp->type);
					break;
				}
			}
			break;
			default:
				showtrace("unhandle ipv4 type : 0x%02x\n", ipv4->protocol);
				break;
			}
		}
		break;
		default:
			showtrace("unhandle eth type : 0x%04x\n", bpf_ntohs(eth->h_proto));
			break;
		}
	} while (0);

out:

	// data report
	value = bpf_map_lookup_elem(&xdp_stats_map, &action);
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

	return action;
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