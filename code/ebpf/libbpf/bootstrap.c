/*
 * @Author: gongluck
 * @Date: 2023-05-02 21:23:48
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-02 22:06:42
 */

// gcc -g -O2 bootstrap.c -o bootstrap -L /usr/lib/$(uname -m)-linux-gnu -l:libbpf.a -lelf -lz

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <bpf/libbpf.h>
#include "bootstrap.h"
#include "bootstrap.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

static volatile bool exiting = false;
static void sig_handler(int sig)
{
    exiting = true;
}

static int handle_event(void *ctx, void *data, size_t data_sz)
{
    char ts[32];
    time_t t;
    time(&t);
    struct tm *tm = localtime(&t);
    strftime(ts, sizeof(ts), "%H:%M:%S", tm);

    const struct myevent *myevent = data;
    if (myevent->exit_event)
    {
        printf("%-8s %-5s %-16s %-7d %-7d [%u]", ts, "EXIT", myevent->comm, myevent->pid, myevent->tgid, myevent->exit_code);
        if (myevent->duration_ns)
        {
            printf(" (%llums)", myevent->duration_ns / 1000000);
        }
        printf("\n");
    }
    else
    {
        printf("%-8s %-5s %-16s %-7d %-7d %s\n", ts, "EXEC", myevent->comm, myevent->pid, myevent->tgid, myevent->filename);
    }
    return 0;
}

int main(int argc, char **argv)
{
    libbpf_set_print(libbpf_print_fn);

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    struct bootstrap_bpf *skel = bootstrap_bpf__open_and_load();
    if (skel == NULL)
    {
        fprintf(stderr, "Failed to open and load BPF skeleton\n");
        goto cleanup;
    }

    int err = bootstrap_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    struct ring_buffer *ringbuf = ring_buffer__new(bpf_map__fd(skel->maps.event_ringbuf), handle_event, NULL, NULL);
    if (ringbuf == NULL)
    {
        err = -1;
        fprintf(stderr, "Failed to create ring buffer\n");
        goto cleanup;
    }

    printf("%-8s %-5s %-16s %-7s %-7s %s\n", "TIME", "EVENT", "COMM", "PID", "TGID", "FILENAME/EXIT CODE");
    while (!exiting)
    {
        err = ring_buffer__poll(ringbuf, 100 /* timeout, ms */);
        if (err < 0)
        {
            printf("Error polling ring buffer: %d\n", err);
            break;
        }
    }

cleanup:
    if (ringbuf != NULL)
    {
        ring_buffer__free(ringbuf);
        ringbuf = NULL;
    }

    if (skel != NULL)
    {
        bootstrap_bpf__destroy(skel);
    }

    return err < 0 ? -err : 0;
}