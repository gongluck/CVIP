/*
 * @Author: gongluck 
 * @Date: 2020-11-13 18:53:49 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-13 19:15:37
 */

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc hiredis.c -I /usr/local/include/hiredis -L /usr/local/lib -lhiredis

int main()
{
    // 连接Redis服务
    redisContext *context = redisConnect("127.0.0.1", 6379);
    if (context == NULL || context->err)
    {
        if (context)
        {
            printf("%s\n", context->errstr);
        }
        else
        {
            printf("redisConnect error\n");
        }
        exit(EXIT_FAILURE);
    }
    printf("-----------------connect success--------------------\n");

    // REDIS_REPLY_STRING == 1 :返回值是字符串,字符串储存在redis->str当中,字符串长度为redis->len。
    // REDIS_REPLY_ARRAY == 2 :返回值是数组,数组大小存在redis->elements里面,数组值存储在redis->element[i]里面。数组里面存储的是指向redisReply的指针,数组里面的返回值可以通过redis->element[i]->str来访问,数组的结果里全是type==REDIS_REPLY_STRING的redisReply对象指针。
    // REDIS_REPLY_INTEGER == 3 :返回值为整数long long。
    // REDIS_REPLY_NIL == 4 :返回值为空表示执行结果为空。
    // REDIS_REPLY_STATUS == 5 :返回命令执行的状态,比如set foo bar返回的状态为OK,存储在str当中reply->str == "OK"。
    // REDIS_REPLY_ERROR == 6 :命令执行错误,错误信息存放在reply->str当中。

    // 授权
    redisReply *reply = redisCommand(context, "auth gongluck");
    printf("type : %d\n", reply->type);
    if (reply->type == REDIS_REPLY_STATUS)
    {
        printf("auth ok\n");
    }
    else if (reply->type == REDIS_REPLY_ERROR)
    {
        printf("auth err : %s\n", reply->str);
    }
    freeReplyObject(reply);

    // Set Key Value
    char *key = "str";
    char *val = "Hello World";
    reply = redisCommand(context, "SET %s %s", key, val);
    printf("type : %d\n", reply->type);
    if (reply->type == REDIS_REPLY_STATUS)
    {
        printf("SET %s %s\n", key, val);
    }
    freeReplyObject(reply);

    // GET Key
    reply = redisCommand(context, "GET %s", key);
    if (reply->type == REDIS_REPLY_STRING)
    {
        printf("GET str %s\n", reply->str);
        printf("GET len %ld\n", reply->len);
    }
    freeReplyObject(reply);

    // APPEND key value
    char *append = " I am your GOD";
    reply = redisCommand(context, "APPEND %s %s", key, append);
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("APPEND %s %s \n", key, append);
    }
    freeReplyObject(reply);

    reply = redisCommand(context, "GET %s", key);
    if (reply->type == REDIS_REPLY_STRING)
    {
        printf("GET %s\n", reply->str);
    }
    freeReplyObject(reply);

    // INCR key
    reply = redisCommand(context, "INCR counter");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("INCR counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);
    reply = redisCommand(context, "INCR counter");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("INCR counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);

    // DECR key
    reply = redisCommand(context, "DECR counter");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("DECR counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);
    reply = redisCommand(context, "DECR counter");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("DECR counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);

    // DECRBY key decrement
    reply = redisCommand(context, "DECRBY counter 5");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("DECRBY counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);
    reply = redisCommand(context, "DECRBY counter 5");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("DECRBY counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);

    // INCRBY key increment
    reply = redisCommand(context, "INCRBY counter 5");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("INCRBY counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);

    reply = redisCommand(context, "INCRBY counter 5");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("INCRBY counter %lld\n", reply->integer);
    }
    freeReplyObject(reply);

    // GETRANGE key start end
    reply = redisCommand(context, "GETRANGE str 0 5");
    if (reply->type == REDIS_REPLY_STRING)
    {
        printf("GETRANGE %s %s\n", key, reply->str);
    }
    freeReplyObject(reply);

    // GETSET key value
    reply = redisCommand(context, "GETSET %s %s", key, val);
    if (reply->type == REDIS_REPLY_STRING)
    {
        printf("GETSET %s %s\n", key, reply->str);
    }
    freeReplyObject(reply);

    /*INCRBYFLOAT key increment*/
    reply = redisCommand(context, "INCRBYFLOAT f 2.1");
    if (reply->type == REDIS_REPLY_STRING)
    {
        printf("INCRBYFLOAT counter %s\n", reply->str);
    }
    freeReplyObject(reply);

    /*MSET key value [key value ...]*/
    reply = redisCommand(context, "MSET k1 hello k2 world k3 good");
    if (reply->type == REDIS_REPLY_STATUS)
    {
        printf("MSET k1 hello k2 world k3 good\n");
    }
    freeReplyObject(reply);

    /*MGET key [key ...]*/
    reply = redisCommand(context, "MGET k1 k2 k3");
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        printf("MGET k1  k2  k3 \n");
        redisReply **pReply = reply->element;
        int i = 0;
        size_t len = reply->elements;
        //hello world good
        for (; i < len; ++i)
        {
            printf("%s ", pReply[i]->str);
        }
        printf("\n");
    }
    freeReplyObject(reply);

    /*STRLEN key*/
    reply = redisCommand(context, "STRLEN str");
    if (reply->type == REDIS_REPLY_INTEGER)
    {
        printf("STRLEN str %lld \n", reply->integer);
    }
    freeReplyObject(reply);

    /*SETEX key seconds value*/
    reply = redisCommand(context, "SETEX s 10 10seconds");
    if (reply->type == REDIS_REPLY_STATUS)
    {
        printf("SETEX s 10 10seconds\n");
        freeReplyObject(reply);
        int i = 0;
        while (i++ < 12)
        {
            reply = redisCommand(context, "GET s");
            if (reply->type == REDIS_REPLY_STRING)
            {
                printf("%d s %s\n", i, reply->str);
            }
            else if (reply->type == REDIS_REPLY_NIL)
            {
                printf("%d s nil\n", i);
            }
            freeReplyObject(reply);
            sleep(1);
        }
    }

    redisFree(context);
    return EXIT_SUCCESS;
}
