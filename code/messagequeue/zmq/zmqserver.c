/*
 * @Author: gongluck 
 * @Date: 2020-11-24 09:49:11 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-24 10:16:56
 */

// gcc zmqserver.c -lzmq

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    // Socket to talk to clients
    void *context = zmq_ctx_new();
    // 与客户端通信的套接字
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    assert(rc == 0);

    int got = 0;
    while (1)
    {
        // 等待客户端请求
        char buffer[10];
        got = zmq_recv(responder, buffer, 10, 0);
        printf("收到 %.*s\n", got<10?got:10, buffer);
        // 返回应答
        zmq_send(responder, "RECVED", 6, 0);
    }
    return 0;
}
