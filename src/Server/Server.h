#pragma once

class EventLoop;
class Socket;
class Acceptor;
class Server
{
public:
    Server(EventLoop *loop);
    ~Server();

    void NewConnection(Socket *serv_sock);
    void HandleReadEvent(int sockfd);

private:
    EventLoop *m_loop;
    Acceptor *m_acceptor;
};