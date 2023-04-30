/*
 * @Author: gongluck
 * @Date: 2023-04-30 23:19:34
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-30 23:34:34
 */

// gcc -g -O2 minimal.c -o minimal -L /usr/lib/$(uname -m)-linux-gnu -l:libbpf.a -lelf -lz

#include <stdio.h>
#include <unistd.h>
#include <bpf/libbpf.h>

// BTF
#include "minimal.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
    struct minimal_bpf *skel;
    int err;

    libbpf_set_print(libbpf_print_fn);

    skel = minimal_bpf__open();
    if (!skel)
    {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    skel->bss->my_pid = getpid();

    err = minimal_bpf__load(skel);
    if (err)
    {
        fprintf(stderr, "Failed to load and verify BPF skeleton\n");
        goto cleanup;
    }

    err = minimal_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF programs.\n");

    for (;;)
    {
        // 触发sys_enter_write
        fprintf(stderr, ".");
        sleep(1);
    }

cleanup:
    minimal_bpf__destroy(skel);
    return -err;
}
