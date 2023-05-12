/*
 * @Author: gongluck
 * @Date: 2023-05-12 15:27:04
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-12 15:37:24
 */

/*
gcc -g tc.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o tc
*/

#include <signal.h>
#include <unistd.h>
#include "tc.skel.h"

#define LO_IFINDEX 1

static volatile sig_atomic_t exiting = 0;

static void sig_int(int signo)
{
    exiting = 1;
}

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
    libbpf_set_print(libbpf_print_fn);

    bool hook_created = false;
    struct bpf_tc_hook tc_hook = {.sz = sizeof(tc_hook), .ifindex = 1, .attach_point = BPF_TC_INGRESS};
    struct bpf_tc_opts tc_opts = {.sz = sizeof(tc_opts), .handle = 1, .priority = 1};
    struct tc_bpf *skel = tc_bpf__open_and_load();
    if (skel == NULL)
    {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    int err = bpf_tc_hook_create(&tc_hook);
    if (err == -EEXIST)
    {
        hook_created = true;
        fprintf(stderr, "EEXIST\n");
    }
    else if (err != 0)
    {
        fprintf(stderr, "Failed to create TC hook: %d\n", err);
        goto cleanup;
    }

    tc_opts.prog_fd = bpf_program__fd(skel->progs.tc_ingress);
    err = bpf_tc_attach(&tc_hook, &tc_opts);
    if (err != 0)
    {
        fprintf(stderr, "Failed to attach TC: %d\n", err);
        goto cleanup;
    }

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        err = errno;
        fprintf(stderr, "Can't set signal handler: %s\n", strerror(errno));
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF program.\n");

    while (!exiting)
    {
        fprintf(stderr, ".");
        sleep(1);
    }

    tc_opts.flags = tc_opts.prog_fd = tc_opts.prog_id = 0;
    err = bpf_tc_detach(&tc_hook, &tc_opts);
    if (err)
    {
        fprintf(stderr, "Failed to detach TC: %d\n", err);
        goto cleanup;
    }

cleanup:
    if (hook_created)
    {
        bpf_tc_hook_destroy(&tc_hook);
    }
    tc_bpf__destroy(skel);
    return -err;
}
