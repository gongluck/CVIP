/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:44:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/poll.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024
#define POLL_SIZE 1024

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

    struct pollfd fds[POLL_SIZE] = {0};
    fds[0].fd = listenfd;
    fds[0].events = POLLIN;

    int max_fd = 0, i = 0;
    for (i = 1; i < POLL_SIZE; i++)
    {
        fds[i].fd = -1;
    }

    while (1)
    {
        int nready = poll(fds, max_fd + 1, 5);
        if (nready <= 0)
            continue;

        if ((fds[0].revents & POLLIN) == POLLIN) // 判断listenfd是否有数据可读(新连接)
        {
            struct sockaddr_in client_addr = {0};
            socklen_t client_len = sizeof(client_addr);

            int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
            if (clientfd <= 0)
                continue;

            char str[INET_ADDRSTRLEN] = {0};
            printf("recvived from %s at port %d, sockfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                   ntohs(client_addr.sin_port), listenfd, clientfd);

            fds[clientfd].fd = clientfd;
            fds[clientfd].events = POLLIN;

            if (clientfd > max_fd)
                max_fd = clientfd;

            if (--nready == 0)
                continue;
        }

        for (i = listenfd + 1; i <= max_fd; i++)
        {
            if (fds[i].revents & (POLLIN | POLLERR))
            {
                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(i, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        printf("read all data");
                    }

                    close(i);
                    fds[i].fd = -1;
                }
                else if (ret == 0)
                {
                    printf(" disconnect %d\n", i);

                    close(i);
                    fds[i].fd = -1;
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
