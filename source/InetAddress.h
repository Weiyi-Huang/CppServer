#pragma onece
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void SetInetAddr(sockaddr_in addr_in, socklen_t addr_len);
    sockaddr_in GetAddr();
    socklen_t GetAddrLen();

private:
    sockaddr_in m_addr_in;
    socklen_t m_addr_len;
};