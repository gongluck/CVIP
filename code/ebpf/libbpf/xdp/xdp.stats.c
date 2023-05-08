/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:29:30
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-08 11:51:56
 */

/*
gcc -g xdp.stats.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o xdp_stats
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <time.h>

#include "xdp.struct.h"

const int sleeptime = 1;

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
    libbpf_set_print(libbpf_print_fn);

    int ret = 0;
    int ifindex = -1;
    int fd = bpf_obj_get(XDPMAPFILE "/xdp_stats_map");
    fprintf(stderr, "%d\n", fd);
    CHECKGOTO((fd >= 0), true, cleanup);

    struct bpf_map_info info;
    unsigned int size = sizeof(info);
    ret = bpf_obj_get_info_by_fd(fd, &info, &size);
    CHECKGOTO(ret, 0, cleanup);

    fprintf(stderr, "name : %s\ntype : %d\nkeysize : %d\nvaluesize : %d\nmaxentries : %d\n", info.name, info.type, info.key_size, info.value_size, info.max_entries);

    __u32 key = XDP_PASS;
    struct xdp_struct lastvalue = {0};
    struct xdp_struct value;
    time_t lastime = time(NULL);
    time_t now;
    while (true)
    {
        switch (MAPTYPE)
        {
        case BPF_MAP_TYPE_ARRAY:
            ret = bpf_map_lookup_elem(fd, &key, &value);
            now = time(NULL);
            if (now - lastime >= 1)
            {
                if (lastvalue.rx_bytes != 0 && lastvalue.rx_packets != 0)
                {
                    fprintf(stderr, "%lfKB/s : %lfpkg/s\n", (value.rx_bytes - lastvalue.rx_bytes) / (1024.0 * sleeptime), (double)(value.rx_packets - lastvalue.rx_packets) / sleeptime);
                }
                lastime = now;
                lastvalue = value;
            }
            break;
        case BPF_MAP_TYPE_PERCPU_ARRAY:
            break;
        default:
            break;
        }
        sleep(sleeptime);
    }
    return 0;

cleanup:
    fprintf(stderr, "cleanup\n");
    return 0;
}