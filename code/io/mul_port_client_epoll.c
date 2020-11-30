/*
 * @Author: gongluck 
 * @Date: 2020-11-30 12:06:22 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-30 13:30:09
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define MAX_BUFFER 128
#define MAX_EPOLLSIZE (384 * 1024)
#define MAX_PORT 100

#define TIME_SUB_MS(tv1, tv2) ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)

int main(int argc, char **argv)
{
	if (argc <= 2)
	{
		printf("Usage: %s ip port\n", argv[0]);
		exit(0);
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int connections = 0;
	char buffer[128] = {0};
	int i = 0, index = 0;

	struct epoll_event events[MAX_EPOLLSIZE];

	int epoll_fd = epoll_create(MAX_EPOLLSIZE);

	strcpy(buffer, "Data From MulClient\n");

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);

	struct timeval tv_begin;
	gettimeofday(&tv_begin, NULL);

	while (1)
	{
		if (++index >= MAX_PORT)
			index = 0;

		struct epoll_event ev;
		int sockfd = 0;

		if (connections < 340000)
		{
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1)
			{
				perror("socket");
				goto err;
			}

			addr.sin_port = htons(port + index);
			if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
			{
				perror("connect");
				goto err;
			}

			int flags = fcntl(sockfd, F_GETFL, 0);
			if (flags >= 0)
			{
				flags |= O_NONBLOCK;
				fcntl(sockfd, F_SETFL, flags);
			}

			int reuse = 1;
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));

			sprintf(buffer, "Hello Server: client --> %d\n", connections);
			send(sockfd, buffer, strlen(buffer), 0);

			ev.data.fd = sockfd;
			ev.events = EPOLLIN | EPOLLOUT;
			epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev);

			connections++;
		}

		if (connections % 1000 == 999 || connections >= 340000)
		{
			struct timeval tv_cur = tv_begin;
			gettimeofday(&tv_begin, NULL);

			int time_used = TIME_SUB_MS(tv_begin, tv_cur);
			printf("connections: %d, sockfd:%d, time_used:%d\n", connections, sockfd, time_used);

			int nfds = epoll_wait(epoll_fd, events, connections, 100);
			for (i = 0; i < nfds; i++)
			{
				int clientfd = events[i].data.fd;

				if (events[i].events & EPOLLOUT)
				{
					sprintf(buffer, "data from %d\n", clientfd);
					send(sockfd, buffer, strlen(buffer), 0);
				}
				else if (events[i].events & EPOLLIN)
				{
					char rBuffer[MAX_BUFFER] = {0};
					ssize_t length = recv(sockfd, rBuffer, MAX_BUFFER, 0);
					if (length > 0)
					{
						printf(" RecvBuffer:%s\n", rBuffer);
					}
					else if (length == 0)
					{
						printf(" Disconnect clientfd:%d\n", clientfd);
						connections--;
						close(clientfd);
					}
					else
					{
						if (errno == EINTR)
							continue;

						printf(" Error clientfd:%d, errno:%d\n", clientfd, errno);
						close(clientfd);
					}
				}
				else
				{
					printf(" clientfd:%d, errno:%d\n", clientfd, errno);
					close(clientfd);
				}
			}
		}
	}

	return 0;

err:
	printf("error : %s\n", strerror(errno));
	return 0;
}
