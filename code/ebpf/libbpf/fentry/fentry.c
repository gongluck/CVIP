/*
 * @Author: gongluck
 * @Date: 2023-05-03 22:18:53
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-03 22:23:31
 */

/*
gcc -g fentry.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o fentry
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <bpf/libbpf.h>
#include "fentry.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

static volatile sig_atomic_t stop;

void sig_int(int signo)
{
    stop = 1;
}

int main(int argc, char **argv)
{
    struct fentry_bpf *skel;
    int err;

    libbpf_set_print(libbpf_print_fn);

    skel = fentry_bpf__open_and_load();
    if (!skel)
    {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    err = fentry_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF programs.\n");

    while (!stop)
    {
        fprintf(stderr, ".");
        sleep(1);
    }

cleanup:
    fentry_bpf__destroy(skel);
    return -err;
}