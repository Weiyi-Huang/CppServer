#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void SetInetAddr(sockaddr_in addr_in);
    sockaddr_in GetAddr();
    char* GetIp();
    uint16_t GetPort();

private:
    sockaddr_in m_addr_in;
};

class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void Bind(InetAddress *addr);
    void Listen();
    void SetNonBlocking();
    
    void Connect(InetAddress *addr);
    int Accept(InetAddress *addr);

    int GetFd();

private:
    int m_fd;
};