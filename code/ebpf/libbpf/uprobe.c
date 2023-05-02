/*
 * @Author: gongluck
 * @Date: 2023-05-02 23:21:55
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-03 00:49:08
 */

// don't use O2
// gcc -g uprobe.c -o uprobe -L /usr/lib64 -l:libbpf.a -lelf -lz

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include "uprobe.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

ssize_t get_uprobe_offset(const void *addr)
{
    size_t start, end, base;
    char buf[256];
    bool found = false;
    FILE *f;

    f = fopen("/proc/self/maps", "r");
    if (!f)
        return -errno;

    while (fscanf(f, "%zx-%zx %s %zx %*[^\n]\n", &start, &end, buf, &base) == 4)
    {
        if (buf[2] == 'x' && (uintptr_t)addr >= start && (uintptr_t)addr < end)
        {
            found = true;
            break;
        }
    }

    fclose(f);

    if (!found)
        return -ESRCH;

    return (uintptr_t)addr - start + base;
}

int uprobed_add(int a, int b)
{
    return a + b;
}

int uprobed_sub(int a, int b)
{
    return a - b;
}

int main(int argc, char **argv)
{
    struct uprobe_bpf *skel;
    long uprobe_offset;
    int err, i;

    libbpf_set_print(libbpf_print_fn);

    skel = uprobe_bpf__open_and_load();
    if (!skel)
    {
        fprintf(stderr, "Failed to open and load BPF skeleton\n");
        return 1;
    }

    // 获取uprobed_add函数文件偏移
    uprobe_offset = get_uprobe_offset(&uprobed_add);

    skel->links.uprobe_add = bpf_program__attach_uprobe(skel->progs.uprobe_add,
                                                        false /* not uretprobe */,
                                                        0 /* self pid */,
                                                        "/proc/self/exe",
                                                        uprobe_offset);
    if (!skel->links.uprobe_add)
    {
        err = -errno;
        fprintf(stderr, "Failed to attach uprobe: %d\n", err);
        goto cleanup;
    }

    skel->links.uretprobe_add = bpf_program__attach_uprobe(skel->progs.uretprobe_add,
                                                           true /* uretprobe */,
                                                           -1 /* any pid */,
                                                           "/proc/self/exe",
                                                           uprobe_offset);
    if (!skel->links.uretprobe_add)
    {
        err = -errno;
        fprintf(stderr, "Failed to attach uprobe: %d\n", err);
        goto cleanup;
    }

    err = uprobe_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to auto-attach BPF skeleton: %d\n", err);
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF programs.\n");

    for (i = 0;; i++)
    {
        fprintf(stderr, ".");
        uprobed_add(i, i + 1);
        uprobed_sub(i * i, i);
        sleep(1);
    }

cleanup:
    uprobe_bpf__destroy(skel);
    return -err;
}
