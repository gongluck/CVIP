/*
 * @Author: gongluck
 * @Date: 2023-05-08 11:32:51
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-18 11:39:05
 */

#include <linux/bpf.h>

#define DEVNAME "eth0"
#define MAPTYPE /*BPF_MAP_TYPE_ARRAY*/ BPF_MAP_TYPE_PERCPU_ARRAY
#define XDPFLAGS (XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE)
#define XDPMAPFILE "/sys/fs/bpf/pining_map"

// 用户态使用
#define CHECKGOTO(ret, tag)                                          \
	if (!(ret))                                                      \
	{                                                                \
		fprintf(stderr, "%s(%d) : %d\n", __FILE__, __LINE__, (ret)); \
		goto tag;                                                    \
	}

// BPF内核态使用
#define BPFCHECKGOTO(ret, tag)                                                  \
	if (!(ret))                                                                 \
	{                                                                           \
		/* bad map relo against '.rodata.str1.1' in section '.rodata.str1.1' */ \
		/*bpf_printk("%s(%d) : %d\n", __FILE__, __LINE__, (ret));*/             \
		goto tag;                                                               \
	}

#define COPYDATA(l, r, etype, ecount)                     \
	{                                                     \
		__builtin_memcpy(&l, &r, sizeof(etype) * ecount); \
	}
#define SWAPDATA(l, r, etype, ecount)    \
	{                                    \
		etype tmp[ecount];               \
		COPYDATA(tmp, l, etype, ecount); \
		COPYDATA(l, r, etype, ecount);   \
		COPYDATA(r, tmp, etype, ecount); \
	}

/*
 0      7 8     15 16    23 24    31
+--------+--------+--------+--------+
|          Source IP Address        |
+--------+--------+--------+--------+
|        Destination IP Address     |
+--------+--------+--------+--------+
|  zero  |  Protocol|    Length     |
+--------+--------+--------+--------+
|                                   |
|        UDP Packet (including      |
|             UDP header)           |
|                                   |
+-----------------------------------+
*/

// TCP/UDP伪首部
struct pseudo_hdr
{
	__u32 src;
	__u32 dst;
	__u8 zero;
	__u8 protocol;
	__u16 len;
} pseudo_hdr;

// 数据统计结构
struct xdp_struct
{
	__u64 rx_packets; // 累计接收包数
	__u64 rx_bytes;	  // 累计接收数据大小
};
