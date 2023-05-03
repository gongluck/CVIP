/*
 * @Author: gongluck
 * @Date: 2023-05-03 01:46:01
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-03 21:55:40
 */

/*
gcc -g usdt.c -L /usr/lib64 -l:libbpf.a -lelf -lz -o usdt
*/

#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <linux/limits.h>
#include "usdt.skel.h"

static volatile sig_atomic_t exiting;
static jmp_buf env;

static void sig_int(int signo)
{
    exiting = 1;
}

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

static void usdt_trigger()
{
    setjmp(env);
}

int main(int argc, char **argv)
{
    struct usdt_bpf_obj *skel;
    int err;

    libbpf_set_print(libbpf_print_fn);

    skel = usdt_bpf_obj__open();
    if (!skel)
    {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    skel->bss->my_pid = getpid();

    err = usdt_bpf_obj__load(skel);
    if (!skel)
    {
        fprintf(stderr, "Failed to load BPF skeleton\n");
        return 1;
    }

    skel->links.usdt_manual_attach = bpf_program__attach_usdt(skel->progs.usdt_manual_attach, getpid(),
                                                              "libc.so.6", "libc", "setjmp", // usdt/libc.so.6:libc:setjmp
                                                              NULL);
    if (!skel->links.usdt_manual_attach)
    {
        err = errno;
        fprintf(stderr, "Failed to attach BPF program `usdt_manual_attach`\n");
        goto cleanup;
    }

    err = usdt_bpf_obj__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        err = errno;
        fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF programs.\n");

    while (!exiting)
    {
        fprintf(stderr, ".");
        usdt_trigger();
        sleep(1);
    }

cleanup:
    usdt_bpf_obj__destroy(skel);
    return -err;
}