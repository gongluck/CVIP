/*
 * @Author: gongluck
 * @Date: 2023-04-30 23:19:34
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-30 23:34:34
 */

// gcc -g -O2 minimal.c -o minimal -L /usr/lib/$(uname -m)-linux-gnu -l:libbpf.a -lelf -lz

#include <stdio.h>
#include <unistd.h>

//  BTF
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

    // skel->bss->my_pid = getpid();//保存静态或全局数据的BSS段

    err = minimal_bpf__load(skel);
    if (err)
    {
        fprintf(stderr, "Failed to load and verify BPF skeleton\n");
        goto cleanup;
    }

    int index = 0;
    int pid = getpid();
    int mapfd = bpf_map__fd(skel->maps.my_pid_map);
    err = bpf_map_update_elem(mapfd, &index, &pid, BPF_ANY);
    if (err < 0)
    {
        fprintf(stderr, "Error updating map with pid: %s\n", strerror(err));
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
