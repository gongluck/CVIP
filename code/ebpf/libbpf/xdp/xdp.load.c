/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:29:30
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-08 12:02:58
 */

/*
gcc -g xdp.load.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o xdp_loader
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

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
    int progfd = -1;
    struct bpf_object *bpfobj = bpf_object__open("xdp.bpf.o");
    CHECKGOTO((bpfobj != NULL), true, cleanup);
    
    ret = bpf_object__load(bpfobj);
    CHECKGOTO(ret, 0, cleanup);

    struct bpf_program *pos;
    printf("BPF object (%s) listing avail --progsec names\n", bpf_object__name(bpfobj));
    bpf_object__for_each_program(pos, bpfobj)
    {
        printf(" %s\n", bpf_program__name(pos));
    }

    struct bpf_program *bpfprog = bpf_object__find_program_by_name(bpfobj, "xdp_pass_func");
    CHECKGOTO((bpfprog != NULL), true, cleanup);

    ifindex = if_nametoindex(DEVNAME);
    progfd = bpf_program__fd(bpfprog);
    fprintf(stderr, "ifindex : %d\nprogfd : %d\n", ifindex, progfd);

    ret = bpf_xdp_attach(ifindex, progfd, XDPFLAGS, NULL);
    CHECKGOTO(ret, 0, cleanup);

    ret = bpf_object__pin_maps(bpfobj, XDPMAPFILE);
    CHECKGOTO(ret, 0, cleanup);

    printf("Successfully\n");
    return 0;

cleanup:
    fprintf(stderr, "cleanup\n");
    if (bpfobj != NULL)
    {
        bpf_object__close(bpfobj);
        bpfobj = NULL;
    }
    return 0;
}