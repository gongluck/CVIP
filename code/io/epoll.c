/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:52:01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024
#define EPOLL_SIZE 1024

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

    int epoll_fd = epoll_create(EPOLL_SIZE);
    struct epoll_event ev, events[EPOLL_SIZE] = {0};

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);

    while (1)
    {
        int nready = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);
        if (nready == -1)
        {
            printf("epoll_wait\n");
            break;
        }

        for (int i = 0; i < nready; i++)
        {
            if (events[i].data.fd == listenfd)
            {
                struct sockaddr_in client_addr = {0};
                socklen_t client_len = sizeof(client_addr);

                int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
                if (clientfd <= 0)
                    continue;

                char str[INET_ADDRSTRLEN] = {0};
                printf("recvived from %s at port %d, sockfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                       ntohs(client_addr.sin_port), listenfd, clientfd);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev);
            }
            else
            {
                int clientfd = events[i].data.fd;

                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(clientfd, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        printf("read all data");
                    }

                    close(clientfd);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = clientfd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);
                }
                else if (ret == 0)
                {
                    printf(" disconnect %d\n", clientfd);

                    close(clientfd);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = clientfd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);

                    break;
                }
                else
                {
                    printf("Recv: %s, %d Bytes\n", buffer, ret);
                }
            }
        }
    }

    return 0;
}
