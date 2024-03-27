#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void SetNonBlocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(sockfd == -1, "Socket create error!");

    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    ErrorIf(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "Socket bind error!");

    ErrorIf(listen(sockfd, SOMAXCONN) == -1, "Socket listen error!");

    int epfd = epoll_create1(0);
    ErrorIf(epfd == -1, "Epoll create error!");

    epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));

    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    SetNonBlocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        ErrorIf(nfds == -1, "Epoll wait error!");
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == sockfd) // Connect event
            {
                sockaddr_in clnt_addr;
                bzero(&clnt_addr, sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);

                int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
                ErrorIf(clnt_sockfd == -1, "Socket accept error!");

                printf("New client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                SetNonBlocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
            }
            else if (events[i].events & EPOLLIN) // Read event
            {
                char buf[READ_BUFFER];
                while (true)
                {
                    bzero(&buf, sizeof(buf));
                    ssize_t read_bytes = read(events[i].data.fd, buf, sizeof(buf));
                    if (read_bytes > 0)
                    {
                        printf("Message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    }
                    else if (read_bytes == -1 && errno == EINTR)
                    {
                        printf("Continue reading\n");
                        continue;
                    }
                    else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                    {
                        printf("Finish reading once, errno: %d\n", errno);
                        break;
                    }
                    else if (read_bytes == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                }
            }
            else
            {
                printf("Something else happened\n");
            }
        }
    }
    close(sockfd);
    return 0;
}