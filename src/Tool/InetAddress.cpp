#include "InetAddress.h"
#include <cstring>

InetAddress::InetAddress() : m_addr_len(sizeof(m_addr_in))
{
    bzero(&m_addr_in, sizeof(m_addr_in));
}

InetAddress::InetAddress(const char *ip, uint16_t port) : m_addr_len(sizeof(m_addr_in))
{
    bzero(&m_addr_in, sizeof(m_addr_in));
    m_addr_in.sin_family = AF_INET;
    m_addr_in.sin_addr.s_addr = inet_addr(ip);
    m_addr_in.sin_port = htons(port);
    m_addr_len = sizeof(m_addr_in);
}

InetAddress::~InetAddress()
{
}

void InetAddress::SetInetAddr(sockaddr_in addr_in, socklen_t addr_len)
{
    m_addr_in = addr_in;
    m_addr_len = addr_len;
}

sockaddr_in InetAddress::GetAddr()
{
    return m_addr_in;
}

socklen_t InetAddress::GetAddrLen()
{
    return m_addr_len;
}
