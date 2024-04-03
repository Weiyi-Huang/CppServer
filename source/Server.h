#pragma once
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
public:
    Server(EventLoop *loop);
    ~Server();

    void NewConnection(Socket *sock);
    void DeleteConnection(int sockfd);

private:
    EventLoop *m_loop;
    Acceptor *m_acceptor;
    std::map<int, Connection*> m_connections;
};