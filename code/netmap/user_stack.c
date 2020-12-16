/*
 * @Author: gongluck 
 * @Date: 2020-12-15 11:02:53 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-15 13:04:41
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/poll.h>
#include <arpa/inet.h>

#define NETMAP_WITH_LIBS

#include <net/netmap_user.h>
#pragma pack(1)

#define ETH_ALEN 6
#define PROTO_IP 0x0800
#define PROTO_ARP 0x0806

#define PROTO_UDP 17
#define PROTO_ICMP 1
#define PROTO_IGMP 2

// 以太网协议头
struct ethhdr
{
	unsigned char h_dest[ETH_ALEN];	  //目的mac地址，6B
	unsigned char h_source[ETH_ALEN]; //源mac地址，6B
	unsigned short h_proto;			  //类型，2B
	unsigned char body[0];			  //数据，46~1500B
	/*unsigned char crc[4];*/		  //CRC，4B
};

// IP协议头
struct iphdr
{
	unsigned char
		version : 4,		//版本，4b
		hdrlen : 4;			//首部长度，4b
	unsigned char tos;		//服务类型，8b
	unsigned short tot_len; //总长度，16b

	unsigned short id; //标识，16b
	unsigned short
		flag : 3,	 //标志，3b
		offset : 13; //片偏移，13b

	unsigned char ttl;		//生存时间，8b
	unsigned char protocol; //协议，8b
	unsigned short check;	//首部校验和，16b

	unsigned int saddr; //源ip，32b
	unsigned int daddr; //目的ip，32b

	unsigned char body[0];
};

// UDP协议头
struct udphdr
{
	unsigned short source; //源端口号，16b
	unsigned short dest;   //目的端口号，16b
	unsigned short len;	   //长度，16b
	unsigned short check;  //校验和，16b

	unsigned char body[0];
};

// UDP包
struct udppkt
{
	struct ethhdr eh;
	struct iphdr ip;
	struct udphdr udp;
	unsigned char body[0];
};

struct arphdr
{
	unsigned short h_type;
	unsigned short h_proto;
	unsigned char h_addrlen;
	unsigned char protolen;
	unsigned short oper;
	unsigned char smac[ETH_ALEN];
	unsigned int sip;
	unsigned char dmac[ETH_ALEN];
	unsigned int dip;
};

struct arppkt
{
	struct ethhdr eh;
	struct arphdr arp;
};

struct icmphdr
{
	unsigned char type;
	unsigned char code;
	unsigned short check;
	unsigned short identifier;
	unsigned short seq;
	unsigned char data[32];
};

struct icmppkt
{
	struct ethhdr eh;
	struct iphdr ip;
	struct icmphdr icmp;
};

void print_mac(unsigned char *mac)
{
	int i;
	for (i = 0; i < ETH_ALEN - 1; i++)
	{
		printf("%02x:", mac[i]);
	}
	printf("%02x", mac[i]);
}

void print_ip(unsigned char *ip)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		printf("%d.", ip[i]);
	}
	printf("%d", ip[i]);
}

void print_arp(struct arppkt *arp)
{
	print_mac(arp->eh.h_dest);
	printf(" ");

	print_mac(arp->eh.h_source);
	printf(" ");

	printf("0x%04x ", ntohs(arp->eh.h_proto));
	printf("  ");
}

int str2mac(char *mac, char *str)
{
	char *p = str;
	unsigned char value = 0x0;
	int i = 0;

	while (p != '\0')
	{
		if (*p == ':')
		{
			mac[i++] = value;
			value = 0x0;
		}
		else
		{
			unsigned char temp = *p;
			if (temp <= '9' && temp >= '0')
			{
				temp -= '0';
			}
			else if (temp <= 'f' && temp >= 'a')
			{
				temp -= 'a';
				temp += 10;
			}
			else if (temp <= 'F' && temp >= 'A')
			{
				temp -= 'A';
				temp += 10;
			}
			else
			{
				break;
			}
			value <<= 4;
			value |= temp;
		}
		p++;
	}

	mac[i] = value;

	return 0;
}

void echo_arp_pkt(struct arppkt *arp, struct arppkt *arp_rt, char *hmac)
{
	memcpy(arp_rt, arp, sizeof(struct arppkt));
	memcpy(arp_rt->eh.h_dest, arp->eh.h_source, ETH_ALEN);
	str2mac(arp_rt->eh.h_source, hmac);
	arp_rt->eh.h_proto = arp->eh.h_proto;

	arp_rt->arp.h_addrlen = 6;
	arp_rt->arp.protolen = 4;
	arp_rt->arp.oper = htons(2);

	str2mac(arp_rt->arp.smac, hmac);
	arp_rt->arp.sip = arp->arp.dip;

	memcpy(arp_rt->arp.dmac, arp->arp.smac, ETH_ALEN);
	arp_rt->arp.dip = arp->arp.sip;
}

void echo_udp_pkt(struct udppkt *udp, struct udppkt *udp_rt)
{
	memcpy(udp_rt, udp, sizeof(struct udppkt));

	memcpy(udp_rt->eh.h_dest, udp->eh.h_source, ETH_ALEN);
	memcpy(udp_rt->eh.h_source, udp->eh.h_dest, ETH_ALEN);

	udp_rt->ip.saddr = udp->ip.daddr;
	udp_rt->ip.daddr = udp->ip.saddr;

	udp_rt->udp.source = udp->udp.dest;
	udp_rt->udp.dest = udp->udp.source;
}

unsigned short in_cksum(unsigned short *addr, int len)
{
	register int nleft = len;
	register unsigned short *w = addr;
	register int sum = 0;
	unsigned short answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(u_char *)(&answer) = *(u_char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return (answer);
}

void echo_icmp_pkt(struct icmppkt *icmp, struct icmppkt *icmp_rt)
{
	memcpy(icmp_rt, icmp, sizeof(struct icmppkt));

	icmp_rt->icmp.type = 0x0; //
	icmp_rt->icmp.code = 0x0; //
	icmp_rt->icmp.check = 0x0;

	icmp_rt->ip.saddr = icmp->ip.daddr;
	icmp_rt->ip.daddr = icmp->ip.saddr;

	memcpy(icmp_rt->eh.h_dest, icmp->eh.h_source, ETH_ALEN);
	memcpy(icmp_rt->eh.h_source, icmp->eh.h_dest, ETH_ALEN);

	icmp_rt->icmp.check = in_cksum((unsigned short *)&icmp_rt->icmp, sizeof(struct icmphdr));
}

int main()
{
	struct ethhdr *eh;
	struct pollfd pfd = {0};
	struct nm_pkthdr h;
	unsigned char *stream = NULL;

	struct nm_desc *nmr = nm_open("netmap:ens33", NULL, 0, NULL);
	if (nmr == NULL)
	{
		return -1;
	}

	pfd.fd = nmr->fd;
	pfd.events = POLLIN;

	while (1)
	{
		int ret = poll(&pfd, 1, -1);
		if (ret < 0)
			continue;

		if (pfd.revents & POLLIN)
		{
			stream = nm_nextpkt(nmr, &h);
			eh = (struct ethhdr *)stream;

			if (ntohs(eh->h_proto) == PROTO_IP)
			{
				struct udppkt *udp = (struct udppkt *)stream;
				if (udp->ip.protocol == PROTO_UDP)
				{
					struct in_addr addr;
					addr.s_addr = udp->ip.saddr;

					int udp_length = ntohs(udp->udp.len);
					printf("%s:%d:length:%d, ip_len:%d --> ", inet_ntoa(addr), udp->udp.source,
						   udp_length, ntohs(udp->ip.tot_len));

					udp->body[udp_length - 8] = '\0';
					printf("udp --> %s\n", udp->body);
#if 1
					struct udppkt udp_rt;
					echo_udp_pkt(udp, &udp_rt);
					nm_inject(nmr, &udp_rt, sizeof(struct udppkt));
#endif
				}
				else if (udp->ip.protocol == PROTO_ICMP)
				{
					struct icmppkt *icmp = (struct icmppkt *)stream;

					printf("icmp ---------- --> %d, %x\n", icmp->icmp.type, icmp->icmp.check);
					if (icmp->icmp.type == 0x08)
					{
						struct icmppkt icmp_rt = {0};
						echo_icmp_pkt(icmp, &icmp_rt);

						//printf("icmp check %x\n", icmp_rt.icmp.check);
						nm_inject(nmr, &icmp_rt, sizeof(struct icmppkt));
					}
				}
				else if (udp->ip.protocol == PROTO_IGMP)
				{
				}
				else
				{
					printf("other ip packet");
				}
			}
			else if (ntohs(eh->h_proto) == PROTO_ARP)
			{
				struct arppkt *arp = (struct arppkt *)stream;
				struct arppkt arp_rt;

				if (arp->arp.dip == inet_addr("192.168.2.217"))
				{
					echo_arp_pkt(arp, &arp_rt, "00:50:56:33:1c:ca");
					nm_inject(nmr, &arp_rt, sizeof(struct arppkt));
				}
			}
		}
	}
}
