#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"
#include <unistd.h>
#include <cstring>

Connection::Connection(EventLoop *loop, Socket *sock) : m_loop(loop), m_sock(sock), m_channel(nullptr),
m_inBuffer(new std::string()), m_readBuffer(nullptr)
{
    m_channel = new Channel(m_loop, m_sock->GetFd());
    std::function<void()> cb = std::bind(&Connection::Echo, this, m_sock->GetFd());
    m_channel->SetCallback(cb);
    m_channel->EnableReading();
    m_readBuffer = new Buffer();
}

Connection::~Connection()
{
    delete m_channel;
    delete m_sock;
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
            printf("Finish reading once, errno: %d\n", errno);
            printf("Message from client fd %d: %s\n", sockfd, m_readBuffer->C_str());
            ErrorIf(write(sockfd, m_readBuffer->C_str(), m_readBuffer->Size()) == -1, "Socket write error");
            m_readBuffer->Clear();
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
