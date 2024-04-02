#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Connection
{
public:
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();

    void Echo(int sockfd);
    void SetDeleteConnectionCallBack(std::function<void(Socket*)> cb);

private:
    EventLoop *m_loop;
    Socket *m_sock;
    Channel *m_channel;
    std::function<void(Socket*)> m_deleteConnectionCallback;
};