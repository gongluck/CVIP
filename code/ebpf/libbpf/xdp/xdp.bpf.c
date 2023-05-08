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
#include "xdp.struct.h"

struct bpf_map_def
{
	__uint(type, MAPTYPE);
	__uint(key_size, sizeof(__u32));
	__uint(value_size, sizeof(struct xdp_struct));
	__uint(max_entries, XDP_REDIRECT + 1);
} xdp_stats_map SEC(".maps");

SEC("xdp")
int xdp_pass_func(struct xdp_md *ctx)
{
	bpf_printk("pass");

	__u32 key = XDP_PASS;
	struct xdp_struct *value = bpf_map_lookup_elem(&xdp_stats_map, &key);
	if (value == NULL)
	{
		return XDP_ABORTED;
	}
	int size = ctx->data_end - ctx->data;
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