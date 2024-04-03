#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

Connection::Connection(EventLoop *loop, Socket *sock) : m_loop(loop), m_sock(sock), m_channel(nullptr),
                                                        m_inBuffer(new std::string()), m_readBuffer(nullptr)
{
    m_channel = new Channel(m_loop, m_sock->GetFd());
    m_channel->EnableRead();
    m_channel->UseET();
    std::function<void()> cb = std::bind(&Connection::Echo, this, m_sock->GetFd());
    m_channel->SetReadCallback(cb);
    m_channel->SetUseThreadPool(true);

    m_readBuffer = new Buffer();
}

Connection::~Connection()
{
    delete m_channel;
    delete m_sock;
    delete m_readBuffer;
}

void Connection::Echo(int sockfd)
{
    char buf[1024];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            m_readBuffer->Append(buf, bytes_read);
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
            printf("Message from client fd %d: %s\n", sockfd, m_readBuffer->C_str());
            Send(sockfd);
            m_readBuffer->Clear();
            break;
        }
        // EOF: The client is disconnected
        else if (bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            m_deleteConnectionCallback(sockfd);
            break;
        }
        else
        {
            printf("Connection reset by peer\n");
            m_deleteConnectionCallback(sockfd);
            break;
        }
    }
}

void Connection::SetDeleteConnectionCallBack(std::function<void(int)> cb)
{
    m_deleteConnectionCallback = cb;
}

void Connection::Send(int sockfd)
{
    char buf[m_readBuffer->Size()];
    strcpy(buf,m_readBuffer->C_str());
    int data_size = m_readBuffer->Size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN)
        {
            break;
        }
        data_left -= bytes_write;
    }
}
