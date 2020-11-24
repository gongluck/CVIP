/*
 * @Author: gongluck 
 * @Date: 2020-11-24 09:52:58 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-24 10:17:50
 */

//gcc zmqclient.c -lzmq

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Connecting to zmq server...\n");
    void *context = zmq_ctx_new();
    // 连接⾄服务端的套接字
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    char buffer[10];
    int got = 0;
    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {  
        printf("正在发送 %d...\n", request_nbr);
        zmq_send(requester, "Hello", 5, 0);
        got = zmq_recv(requester, buffer, 10, 0);
        printf("接收到 %.*s\n", got<10?got:10, buffer);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}
