/*
 * @Author: gongluck 
 * @Date: 2020-12-10 02:27:44 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-10 02:37:16
 */

// gcc -o server server.c -lzmq -g -O0

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main()
{
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");

    while (1)
    {
        char buffer[10];
        int ret = zmq_recv(responder, buffer, 10, 0);
        printf("收到%.*s\n", ret, buffer);
        zmq_send(responder, "server recved.", strlen("server recved."), 0);
    }
    
    return 0;
}
