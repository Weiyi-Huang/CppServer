#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"

#define READ_BUFFER 1024

void HandleReadEvent(int sockfd);

int main()
{
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->Bind(serv_addr);
    serv_sock->Listen();

    Epoll *ep = new Epoll();
    serv_sock->SetNonBlocking();
    ep->AddFd(serv_sock->GetFd(), EPOLLIN | EPOLLET);

    while (true)
    {
        std::vector<epoll_event> events = ep->Poll();
        int nfds = events.size();
        for (int i = 0; i < nfds; ++i)
        {
            // New client connect
            if (events[i].data.fd == serv_sock->GetFd())
            {
                InetAddress *clnt_addr = new InetAddress();
                Socket *clnt_sock = new Socket(serv_sock->Accept(clnt_addr));

                printf("New client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->m_addr.sin_addr), ntohs(clnt_addr->m_addr.sin_port));

                clnt_sock->SetNonBlocking();
                ep->AddFd(clnt_sock->GetFd(), EPOLLIN | EPOLLET);
            }
            // Read event
            else if (events[i].events & EPOLLIN)
            {
                HandleReadEvent(events[i].data.fd);
            }
            else
            {
                printf("Something else happened\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void HandleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("Message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("Continue reading\n");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("Finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}
