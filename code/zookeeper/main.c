#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "zookeeper.h"
#include "zookeeper_log.h"

typedef struct zk_mutex_s
{
    int seq_me;     // 序号
    int seq_watch;  // 监听的序号
    char watch[64]; // 监听的节点名
} zk_mutex_t;
static zk_mutex_t zmt = {0};
static int quit = 0;

/*
typedef void (*void_completion_t)(int rc, const void *data);
*/
void zk_delete_lock_res1_children(int rc, const void *data)
{
    printf("zk_delete_lock_res1_children rc = %d\n", rc);
    quit = 1;
}

/*
typedef void (*stat_completion_t)(int rc, const struct Stat *stat,
        const void *data);
*/
void zk_watch_children(int rc, const struct Stat *stat, const void *data)
{
    printf("zk_watch_children_disappear rc = %d\n", rc);
}

/*
typedef void (*strings_completion_t)(int rc,
        const struct String_vector *strings, const void *data);
*/
void zk_get_children_lock_res1(int rc, const struct String_vector *strings, const void *data)
{
    printf("zk_get_children_lock_res1 rc = %d\n", rc);
    if (rc == 0)
    {
        int i, v, n;
        n = 0;
        char *value_self = NULL;
        char *value_other = NULL;
        zhandle_t *zk_hdl = (zhandle_t *)data;
        for (i = 0; i < strings->count; i++)
        {
            sscanf(strings->data[i], "%d", &v);
            if (v < zmt.seq_me)
            {
                if (n == 0)
                {
                    zmt.seq_watch = v;
                    value_other = strings->data[i];
                    n++;
                }
                else if (n > 0 && v > zmt.seq_watch)
                {
                    zmt.seq_watch = v;
                    value_other = strings->data[i];
                    n++;
                }
            }
            else if (v == zmt.seq_me)
            {
                value_self = strings->data[i];
            }
        }

        if (n == 0)
        {
            // 说明自己已经最小的了
            char temp[64] = {0};
            sprintf(temp, "/lock/res1/%s", value_self);
            printf("%s 获取锁, 获取执行权, 释放锁\n", temp);
            zoo_adelete(zk_hdl, temp, -1, zk_delete_lock_res1_children, zk_hdl);
        }
        else if (n > 0)
        { // 找到可以监听的对象
            memset(zmt.watch, 0, 64 * sizeof(char));
            sprintf(zmt.watch, "/lock/res1/%s", value_other);
            zoo_aexists(zk_hdl, zmt.watch, 1, zk_watch_children, zk_hdl);
        }
    }
}

/*
typedef void
        (*string_completion_t)(int rc, const char *value, const void *data);
*/
void zk_create_lock_res1_es(int rc, const char *value, const void *data)
{
    printf("zk_create_lock_res1_es rc = %d\n", rc);
    if (rc == 0)
    {
        zhandle_t *zk_hdl = (zhandle_t *)data;
        int seq;
        sscanf(value, "/lock/res1/%d", &seq);
        printf("\tname = %s seq = %d\n", value, seq);
        zmt.seq_me = seq;
        if (seq > 0)
        {
            int ret;
            ret = zoo_aget_children(zk_hdl, "/lock/res1", 0, zk_get_children_lock_res1, data);
            if (ret)
            {
                printf("error: zk_create_lock_res1_es:zoo_aget_children\n");
                exit(EXIT_FAILURE);
            }
            return;
        }
        printf("%s 获取锁, 获取执行权, 释放锁\n", value);
        zoo_adelete(zk_hdl, value, -1, zk_delete_lock_res1_children, "");
    }
}

/*
typedef void (*watcher_fn)(zhandle_t *zh, int type,
        int state, const char *path,void *watcherCtx);
*/
void zk_watcher_disconnect(zhandle_t *zh, int type, int state, const char *path, void *ctx)
{
    if (0 == strcmp(zmt.watch, path))
    {
        int ret = zoo_aget_children(zh, "/lock/res1", 0, zk_get_children_lock_res1, zh);
        if (ret)
        {
            printf("error: zk_watcher_disconnect:zoo_aget_children\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, const char *argv[])
{
    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
    zoo_set_log_stream(stdout);

    /* 初始化 zk */
    zhandle_t *zk_hdl = zookeeper_init("127.0.0.1:2181",
                                       zk_watcher_disconnect, 30000, 0, "zookeeper for distribute mutex.", 0);
    if (!zk_hdl)
    {
        printf("error: connecting to zookeeper server...\n");
        exit(EXIT_FAILURE);
    }

    int ret;
    /* 创建 /lock/res1/ 短暂顺序节点*/
    ret = zoo_acreate(zk_hdl, "/lock/res1/", "mark", 5, &ZOO_OPEN_ACL_UNSAFE,
                      ZOO_EPHEMERAL_SEQUENTIAL, zk_create_lock_res1_es, zk_hdl);
    if (ret)
    {
        printf("error: create /lock/res1 EPHEMERAL SEQUENTIAL\n");
        exit(EXIT_FAILURE);
    }
    ret = 0;

    for (;;)
    {
        if (quit)
            break;
        usleep(2500);
    }
    zookeeper_close(zk_hdl);
    return 0;
}
