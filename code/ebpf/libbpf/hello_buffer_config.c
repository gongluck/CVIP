/*
 * @Author: gongluck
 * @Date: 2023-04-29 17:52:03
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-04-29 18:00:18
 */

// gcc -Wall -o hello_buffer_config hello_buffer_config.c -L /usr/lib/$(uname -m)-linux-gnu -l:libbpf.a -lelf -lz

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <bpf/libbpf.h>
#include "hello_buffer_config.h"
#include "hello_buffer_config.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    if (level >= LIBBPF_DEBUG)
        return 0;

    return vfprintf(stderr, format, args);
}

void handle_event(void *ctx, int cpu, void *data, unsigned int data_sz)
{
    struct data_t *m = data;

    printf("%-6d %-16s %-16s %s\n", m->pid, m->command, m->path, m->message);
}

void lost_event(void *ctx, int cpu, long long unsigned int data_sz)
{
    printf("lost event\n");
}

int main()
{
    printf("run at %d\n", __LINE__);

    struct hello_buffer_config_bpf *skel;
    int err;
    struct perf_buffer *pb = NULL;

    libbpf_set_print(libbpf_print_fn);
    printf("run at %d\n", __LINE__);
    skel = hello_buffer_config_bpf__open_and_load();
    if (!skel)
    {
        printf("Failed to open BPF object\n");
        return 1;
    }
    printf("run at %d\n", __LINE__);
    err = hello_buffer_config_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton: %d\n", err);
        hello_buffer_config_bpf__destroy(skel);
        return 1;
    }
    printf("run at %d\n", __LINE__);
    pb = perf_buffer__new(bpf_map__fd(skel->maps.output), 8, handle_event);
    if (!pb)
    {
        err = -1;
        fprintf(stderr, "Failed to create ring buffer\n");
        hello_buffer_config_bpf__destroy(skel);
        return 1;
    }

    printf("run at %d\n", __LINE__);
    while (true)
    {
        err = perf_buffer__poll(pb, 100 /* timeout, ms */);
        // Ctrl-C gives -EINTR
        if (err == -EINTR)
        {
            err = 0;
            break;
        }
        if (err < 0)
        {
            printf("Error polling perf buffer: %d\n", err);
            break;
        }
    }
    printf("run at %d\n", __LINE__);
    perf_buffer__free(pb);
    hello_buffer_config_bpf__destroy(skel);
    return -err;
}