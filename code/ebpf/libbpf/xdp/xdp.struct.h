/*
 * @Author: gongluck
 * @Date: 2023-05-08 11:32:51
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-09 18:19:08
 */

#include <linux/bpf.h>

#define DEVNAME "eth0"
#define MAPTYPE /*BPF_MAP_TYPE_ARRAY*/ BPF_MAP_TYPE_PERCPU_ARRAY
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