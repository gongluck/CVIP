/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:29:30
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-18 11:45:10
 */

/*
gcc -g xdp.unload.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o xdp_unloader
*/

#include "xdp.struct.h"
#include "xdp.skel.h"

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

    ret = xdp_bpf__load(skel);
    CHECKGOTO((ret == 0), cleanup);

    int ifindex = if_nametoindex(DEVNAME);
    fprintf(stderr, "ifindex : %d\n", ifindex);

    ret = bpf_xdp_detach(ifindex, XDPFLAGS, NULL);
    CHECKGOTO((ret == 0), cleanup);

    ret = bpf_object__unpin_maps(skel->obj, XDPMAPFILE);
    CHECKGOTO((ret == 0), cleanup);

    printf("Successfully\n");
    goto close;

cleanup:
    fprintf(stderr, "cleanup\n");

close:
    if (skel != NULL)
    {
        xdp_bpf__destroy(skel);
        skel = NULL;
    }
    return 0;
}