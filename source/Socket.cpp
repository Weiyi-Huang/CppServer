#include "Socket.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <errno.h>

InetAddress::InetAddress()
{
    bzero(&m_addr_in, sizeof(m_addr_in));
}

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    bzero(&m_addr_in, sizeof(m_addr_in));
    m_addr_in.sin_family = AF_INET;
    m_addr_in.sin_addr.s_addr = inet_addr(ip);
    m_addr_in.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}

void InetAddress::SetInetAddr(sockaddr_in addr_in)
{
    m_addr_in = addr_in;
}

sockaddr_in InetAddress::GetAddr()
{
    return m_addr_in;
}

char *InetAddress::GetIp()
{
    return inet_ntoa(m_addr_in.sin_addr);
}

uint16_t InetAddress::GetPort()
{
    return ntohs(m_addr_in.sin_port);
}

Socket::Socket() : m_fd(-1)
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(m_fd == -1, "Socket create error!");
}

Socket::Socket(int fd) : m_fd(fd)
{
    ErrorIf(m_fd == -1, "Socket create error!");
}

Socket::~Socket()
{
    if (m_fd != -1)
    {
        close(m_fd);
        m_fd = -1;
    }
}

void Socket::Bind(InetAddress *addr)
{
    sockaddr_in addr_in = addr->GetAddr();
    ErrorIf(::bind(m_fd, (sockaddr *)&addr_in, sizeof(addr_in)) == -1, "Socket bind error!");
}

void Socket::Listen()
{
    ErrorIf(::listen(m_fd, SOMAXCONN) == -1, "Socket listen error!");
}

void Socket::SetNonBlocking()
{
    fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
}

void Socket::Connect(InetAddress *addr)
{
    sockaddr_in addr_in = addr->GetAddr();
    if (fcntl(m_fd, F_GETFL) && O_NONBLOCK)
    {
        while (true)
        {
            int ret = ::connect(m_fd, (sockaddr *)&addr_in, sizeof(addr_in));
            if (ret == 0)
            {
                break;
            }
            else if (ret == -1 && (errno == EINPROGRESS))
            {
                continue;
            }
            else if (ret == -1)
            {
                ErrorIf(true, "Socket connect error!");
            }
        }
    }
    else
    {
        ErrorIf(::connect(m_fd, (sockaddr *)&addr_in, sizeof(addr_in)) == -1, "Socket connect error!");
    }
}

int Socket::Accept(InetAddress *addr)
{
    int clnt_sockfd = -1;
    sockaddr_in addr_in;
    socklen_t addr_len = sizeof(addr_in);
    bzero(&addr_in, sizeof(addr_in));
    if (fcntl(m_fd, F_GETFL) && O_NONBLOCK)
    {
        while (true)
        {
            clnt_sockfd = ::accept(m_fd, (sockaddr *)&addr_in, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                printf("No connection yet\n");
                continue;
            }
            else if (clnt_sockfd == -1)
            {
                ErrorIf(true, "Socket accept error!");
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        clnt_sockfd = ::accept(m_fd, (sockaddr *)&addr_in, &addr_len);
        ErrorIf(clnt_sockfd == -1, "Socket accept error!");
    }
    addr->SetInetAddr(addr_in);
    return clnt_sockfd;
}

int Socket::GetFd()
{
    return m_fd;
}