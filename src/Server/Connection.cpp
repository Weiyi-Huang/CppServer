#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *sock) : m_loop(loop), m_sock(sock), m_channel(nullptr)
{
    m_channel = new Channel(m_loop, m_sock->GetFd());
    std::function<void()> cb = std::bind(&Connection::Echo, this, m_sock->GetFd());
    m_channel->SetCallback(cb);
    m_channel->EnableReading();
}

Connection::~Connection()
{
    delete m_channel;
    delete m_sock;
}

void Connection::Echo(int sockfd)
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
            m_deleteConnectionCallback(m_sock);
            break;
        }
    }
}

void Connection::SetDeleteConnectionCallBack(std::function<void(Socket *)> cb)
{
    m_deleteConnectionCallback = cb;
}
