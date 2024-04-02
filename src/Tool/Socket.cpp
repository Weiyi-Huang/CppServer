#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>

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
    socklen_t addr_len = addr->GetAddrLen();
    ErrorIf(::bind(m_fd, (sockaddr*)&addr_in, addr_len) == -1, "Socket bind error!");
    addr->SetInetAddr(addr_in, addr_len);
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
    socklen_t addr_len = addr->GetAddrLen();
    ErrorIf(::connect(m_fd, (sockaddr*)&addr_in, addr_len) == -1, "Socket connect error!");
    addr->SetInetAddr(addr_in, addr_len);
}

int Socket::Accept(InetAddress *addr)
{
    sockaddr_in addr_in;
    socklen_t addr_len = sizeof(addr_in);
    bzero(&addr_in, sizeof(addr_in));
    int clnt_sockfd = ::accept(m_fd, (sockaddr*)&addr_in, &addr_len);
    ErrorIf(clnt_sockfd == -1, "Socket accept error!");
    addr->SetInetAddr(addr_in, addr_len);
    return clnt_sockfd;
}

int Socket::GetFd()
{
    return m_fd;
}