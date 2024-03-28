#pragma onece
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

public:
    sockaddr_in m_addr;
    socklen_t m_addr_len;
};