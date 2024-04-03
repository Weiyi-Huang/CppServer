#include "InetAddress.h"
#include <cstring>

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
