#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> //ethhdr
#include <netinet/ip.h>       //iphdr

#define BUFFER_MAX 2048

int main(int argc, char *argv[])
{
  int sock;
  char buffer[BUFFER_MAX];
  int len;

  struct ethhdr *mac_hdr;
  struct iphdr *ip_hdr;
  char *p;

  if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    std::cerr << "socket error : " << errno << std::endl;
    exit(errno);
  }

  while (true)
  {
    len = recvfrom(sock, buffer, BUFFER_MAX, 0, nullptr, nullptr);
    std::cout << "packet len : " << len << std::endl;
    if (len < sizeof(ethhdr) + sizeof(ip_hdr))
    {
      std::cerr << "len error." << std::endl;
      close(sock);
      exit(errno);
    }

    mac_hdr = (struct ethhdr *)buffer;
    ip_hdr = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    printf("src MAC:%X:%X:%X:%X:%X:%X",
           (u_char)mac_hdr->h_source[0],
           (u_char)mac_hdr->h_source[1],
           (u_char)mac_hdr->h_source[2],
           (u_char)mac_hdr->h_source[3],
           (u_char)mac_hdr->h_source[4],
           (u_char)mac_hdr->h_source[5]);

    printf(" ==> dst MAC:%X:%X:%X:%X:%X\n",
           (u_char)mac_hdr->h_dest[0],
           (u_char)mac_hdr->h_dest[1],
           (u_char)mac_hdr->h_dest[2],
           (u_char)mac_hdr->h_dest[3],
           (u_char)mac_hdr->h_dest[4],
           (u_char)mac_hdr->h_dest[5]);

    p = (char *)&ip_hdr->saddr;
    printf("src IP: %d.%d.%d.%d",
           (u_char)p[0],
           (u_char)p[1],
           (u_char)p[2],
           (u_char)p[3]);

    p = (char *)&ip_hdr->daddr;
    printf(" ==> dst IP: %d.%d.%d.%d\n",
           (u_char)p[0],
           (u_char)p[1],
           (u_char)p[2],
           (u_char)p[3]);

    std::cout << "protocol type : ";
    switch (ip_hdr->protocol)
    {
    case IPPROTO_ICMP:
      std::cout << "ICMP" << std::endl;
      break;
    case IPPROTO_IGMP:
      std::cout << "IGMP" << std::endl;
      break;
    case IPPROTO_IPIP:
      std::cout << "IPIP" << std::endl;
      break;
    case IPPROTO_TCP:
      std::cout << "TCP" << std::endl;
      break;
    case IPPROTO_UDP:
      std::cout << "UDP" << std::endl;
      break;
    case IPPROTO_RAW:
      std::cout << "RAW" << std::endl;
      break;
    default:
      std::cout << "Unknown " << (int)ip_hdr->protocol << std::endl;
      break;
    }
    std::cout << std::endl;
  }

  close(sock);
  return 0;
}