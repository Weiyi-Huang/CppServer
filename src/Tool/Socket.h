#pragma once

class InetAddress;
class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void Bind(InetAddress *addr);
    void Listen();
    void SetNonBlocking();

    int Accept(InetAddress *addr);

    int GetFd();

private:
    int m_fd;
};