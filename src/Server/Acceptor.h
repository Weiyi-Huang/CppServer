#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();

    void AcceptConnection();
    void SetNewConnectionCallback(std::function<void(Socket*)> cb);

private:
    EventLoop *m_loop;
    Socket *m_sock;
    InetAddress *m_addr;
    Channel *m_acceptChannel;
    std::function<void(Socket*)> m_newConnectionCallback;
};