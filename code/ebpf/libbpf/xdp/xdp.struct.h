/*
 * @Author: gongluck
 * @Date: 2023-05-08 11:32:51
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-08 12:02:15
 */

#include <linux/bpf.h>
#include <net/if.h>
#include <linux/if_link.h>

#define DEVNAME "lo"
#define MAPTYPE BPF_MAP_TYPE_ARRAY
#define XDPFLAGS (XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE)
#define XDPMAPFILE "/sys/fs/bpf/pining_map"

#define CHECKGOTO(ret, succeed, tag)                                   \
	if (ret != succeed)                                                \
	{                                                                  \
		fprintf(stderr, "line(%d) : %s\n", __LINE__, strerror(errno)); \
		goto tag;                                                      \
	}

struct xdp_struct
{
	__u64 rx_packets;
	__u64 rx_bytes;
};