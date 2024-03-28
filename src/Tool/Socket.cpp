#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

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
    ErrorIf(::bind(m_fd, (sockaddr*)&addr->m_addr, addr->m_addr_len) == -1, "Socket bind error!");
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
    ErrorIf(::connect(m_fd, (sockaddr*)&addr->m_addr, addr->m_addr_len) == -1, "Socket connect error!");
}

int Socket::Accept(InetAddress *addr)
{
    int clnt_sockfd = ::accept(m_fd, (sockaddr*)&addr->m_addr, &addr->m_addr_len);
    ErrorIf(clnt_sockfd == -1, "Socket accept error!");
    return clnt_sockfd;
}

int Socket::GetFd()
{
    return m_fd;
}