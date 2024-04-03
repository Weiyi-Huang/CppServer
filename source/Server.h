#pragma once
#include <map>
#include <vector>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server
{
public:
    Server(EventLoop *loop);
    ~Server();

    void NewConnection(Socket *sock);
    void DeleteConnection(int sockfd);

private:
    EventLoop *m_mainReactor;
    Acceptor *m_acceptor;
    std::map<int, Connection*> m_connections;
    std::vector<EventLoop*> m_subReactors;
    ThreadPool *m_thpool;
};