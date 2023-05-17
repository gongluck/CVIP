/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:29:30
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-16 23:26:54
 */

/*
gcc -g xdp.load.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o xdp_loader
*/

#include "xdp.skel.h"
#include "xdp.struct.h"

#include <net/if.h>        //if_nametoindex
#include <linux/if_link.h> //XDP_FLAGS_SKB_MODE

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
    libbpf_set_print(libbpf_print_fn);

    int ret = 0;

    struct xdp_bpf *skel = xdp_bpf__open();
    CHECKGOTO((skel != NULL), cleanup);

    // 加载前可以修改数据
    skel->bss->g_show_trace = 1;
    // skel->data->g_local_ip = 0x12345678;

    ret = xdp_bpf__load(skel);
    CHECKGOTO((ret == 0), cleanup);

    struct bpf_program *pos;
    printf("BPF object (%s) listing avail --progsec names\n", bpf_object__name(skel->obj));
    bpf_object__for_each_program(pos, skel->obj)
    {
        printf(" %s\n", bpf_program__name(pos));
    }

    struct bpf_program *bpfprog = bpf_object__find_program_by_name(skel->obj, "xdp_pass_func");
    CHECKGOTO((bpfprog != NULL), cleanup);

    int ifindex = if_nametoindex(DEVNAME);
    int progfd = bpf_program__fd(bpfprog);
    fprintf(stderr, "ifindex : %d\nprogfd : %d\n", ifindex, progfd);

    ret = bpf_xdp_attach(ifindex, progfd, XDPFLAGS, NULL);
    CHECKGOTO((ret == 0), cleanup);

    ret = bpf_object__pin_maps(skel->obj, XDPMAPFILE);
    CHECKGOTO((ret == 0), cleanup);

    printf("Successfully\n");
    return 0;

cleanup:
    fprintf(stderr, "cleanup\n");
    if (skel != NULL)
    {
        ret = bpf_xdp_detach(ifindex, XDPFLAGS, NULL);
        ret = bpf_object__unpin_maps(skel->obj, XDPMAPFILE);
        xdp_bpf__destroy(skel);
        skel = NULL;
    }
    return 0;
}