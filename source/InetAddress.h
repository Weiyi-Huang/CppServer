#pragma onece
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void SetInetAddr(sockaddr_in addr_in);
    sockaddr_in GetAddr();

private:
    sockaddr_in m_addr_in;
};