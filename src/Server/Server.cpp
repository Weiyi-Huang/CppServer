#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include <functional>
#include <cstring>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *loop) : m_loop(loop)
{
    m_acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    m_acceptor->SetNewConnectionCallback(cb);
}

Server::~Server()
{
    delete m_acceptor;
}

void Server::NewConnection(Socket *serv_sock)
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(serv_sock->Accept(clnt_addr));
    clnt_sock->SetNonBlocking();

    printf("New client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->m_addr.sin_addr), ntohs(clnt_addr->m_addr.sin_port));

    Channel *clntChannel = new Channel(m_loop, clnt_sock->GetFd());
    std::function<void()> cb = std::bind(&Server::HandleReadEvent, this, clnt_sock->GetFd());
    clntChannel->SetCallback(cb);
    clntChannel->EnableReading();
}

void Server::HandleReadEvent(int sockfd)
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
        // The client interrupts normally and continues to read data
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("Continue reading\n");
            continue;
        }
        // This condition indicates that all data is read using non-blocking IO 
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("Finish reading once, errno: %d\n", errno);
            break;
        }
        // EOF: The client is disconnected
        else if (bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}
