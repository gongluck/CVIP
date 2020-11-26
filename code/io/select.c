/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:39:02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024

int main(int argc, char *argv[])
{
    int port = 9096;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        return -2;
    }
    if (listen(listenfd, 5) < 0)
    {
        perror("listen");
        return -3;
    }

    fd_set rfds, rset;
    FD_ZERO(&rfds);
    FD_SET(listenfd, &rfds);

    int max_fd = listenfd;
    int i = 0;

    while (1)
    {
        rset = rfds;
        int nready = select(max_fd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
        {
            printf("select error : %d\n", errno);
            continue;
        }

        if (FD_ISSET(listenfd, &rset)) // listen的fd可读代表有连接到达
        {
            struct sockaddr_in client_addr = {0};
            socklen_t client_len = sizeof(client_addr);

            int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
            if (clientfd <= 0)
                continue;

            char str[INET_ADDRSTRLEN] = {0};
            printf("recvived from %s at port %d, listenfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                   ntohs(client_addr.sin_port), listenfd, clientfd);

            if (max_fd == FD_SETSIZE) // select的fd上限一般是1024
            {
                printf("clientfd --> out range\n");
                break;
            }
            FD_SET(clientfd, &rfds); // 将新连接fd加入到读取队列中

            if (clientfd > max_fd)
                max_fd = clientfd;

            printf("listenfd:%d, max_fd:%d, clientfd:%d\n", listenfd, max_fd, clientfd);

            if (--nready == 0)
                continue;
        }

        for (i = listenfd + 1; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &rset))
            {
                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(i, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) // 实际可能出现被其他线程读取掉数据的情况
                    {
                        printf("read all data");
                    }
                    FD_CLR(i, &rfds);
                    close(i);
                }
                else if (ret == 0)
                {
                    printf("disconnect %d\n", i);
                    FD_CLR(i, &rfds);
                    close(i);
                    break;
                }
                else
                {
                    printf("Recv: %s, %d Bytes\n", buffer, ret);
                }
                if (--nready == 0)
                    break;
            }
        }
    }

    return 0;
}
