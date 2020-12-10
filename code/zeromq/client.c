/*
 * @Author: gongluck 
 * @Date: 2020-12-10 02:33:38 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-10 02:37:00
 */

// gcc -o client client.c -lzmq -g -O0

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        char buffer[20] = {0};
        zmq_send(requester, "Hello", 5, 0);
        zmq_recv(requester, buffer, 20, 0);
        printf("接收到 %.20s\n", buffer);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}