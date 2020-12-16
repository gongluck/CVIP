#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#include <sys/time.h>
#include <mach/task.h>
#include <mach/mach.h>
#endif
#include "skiplist.h"

static uint32_t
current_time()
{
    uint32_t t;
#if !defined(__APPLE__) || defined(AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER)
    struct timespec ti;
    clock_gettime(CLOCK_MONOTONIC, &ti);
    t = (uint32_t)ti.tv_sec * 1000;
    t += ti.tv_nsec / 1000000;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (uint32_t)tv.tv_sec * 1000;
    t += tv.tv_usec / 1000;
#endif
    return t;
}

zskiplist *init_timer()
{
    return zslCreate();
}

zskiplistNode *add_timer(zskiplist *zsl, uint32_t msec, handler_pt func)
{
    msec += current_time();
    printf("add_timer expire at msec = %u\n", msec);
    return zslInsert(zsl, msec, func);
}

void del_timer(zskiplist *zsl, zskiplistNode *zn)
{
    zslDelete(zsl, zn);
}

void expire_timer(zskiplist *zsl)
{
    zskiplistNode *x;
    uint32_t now = current_time();
    for (;;)
    {
        x = zslMin(zsl);
        if (!x)
            break;
        if (x->score > now)
            break;
        printf("touch timer expire time=%lu, now = %u\n", x->score, now);
        x->handler(x);
        zslDeleteHead(zsl);
    }
}

void print_hello(zskiplistNode *zn)
{
    printf("hello world time = %lu\n", zn->score);
}

int main()
{
    zskiplist *zsl = init_timer();
    add_timer(zsl, 3010, print_hello);
    add_timer(zsl, 3004, print_hello);
    zskiplistNode *zn = add_timer(zsl, 3005, print_hello);
    del_timer(zsl, zn);
    add_timer(zsl, 3008, print_hello);
    add_timer(zsl, 3003, print_hello);
    // zslPrint(zsl);
    for (;;)
    {
        expire_timer(zsl);
        usleep(10000);
    }
    return 0;
}
// nginx keepalive