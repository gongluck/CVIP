/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:29:30
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-09 18:17:45
 */

/*
gcc -g xdp.unload.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o xdp_unloader
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <net/if.h>
#include <linux/if_link.h>
#include "xdp.struct.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
    libbpf_set_print(libbpf_print_fn);

    int ret = 0;
    int ifindex = -1;
    struct bpf_object *bpfobj = bpf_object__open("xdp.bpf.o");
    CHECKGOTO((bpfobj != NULL), true, cleanup);

    ret = bpf_object__load(bpfobj);
    CHECKGOTO(ret, 0, cleanup);

    ifindex = if_nametoindex(DEVNAME);
    fprintf(stderr, "ifindex : %d\n", ifindex);

    ret = bpf_object__unpin_maps(bpfobj, XDPMAPFILE);
    CHECKGOTO(ret, 0, cleanup);

    ret = bpf_xdp_detach(ifindex, XDPFLAGS, NULL);
    CHECKGOTO(ret, 0, cleanup);

    printf("Successfully\n");
    goto close;

cleanup:
    fprintf(stderr, "cleanup\n");

close:
    if (bpfobj != NULL)
    {
        bpf_object__close(bpfobj);
        bpfobj = NULL;
    }
    return 0;
}