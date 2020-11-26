/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:05:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:19:07
 */

// test : nc -uv 127.0.0.1 9096

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

int sockfd = 0;

void do_sigio(int sig)
{
    struct sockaddr_in cli_addr;
    int clilen = sizeof(struct sockaddr_in);

    char buffer[256] = {0};
    int len = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    printf("Message : %s\r\n", buffer);
    int slen = sendto(sockfd, buffer, len, 0, (struct sockaddr *)&cli_addr, clilen);
}

int main(int argc, char *argv[])
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sigaction sigio_action;
    sigio_action.sa_flags = 0;
    sigio_action.sa_handler = do_sigio;
    sigaction(SIGIO, &sigio_action, NULL);//SIGIO call do_sigio

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9096);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    fcntl(sockfd, F_SETOWN, getpid());
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags |= O_ASYNC | O_NONBLOCK;//异步非阻塞
    fcntl(sockfd, F_SETFL, flags);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    while (1)
        sleep(1);

    close(sockfd);

    return 0;
}
