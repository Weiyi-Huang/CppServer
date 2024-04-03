#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "EventLoop.h"
#include <unistd.h>
#include <functional>

Server::Server(EventLoop *loop) : m_mainReactor(loop), m_acceptor(nullptr)
{
    m_acceptor = new Acceptor(m_mainReactor);
    std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    m_acceptor->SetNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();
    m_thpool = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
    {
        m_subReactors.push_back(new EventLoop());
    }

    for (int i = 0; i < size; ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, m_subReactors[i]);
        m_thpool->Add(sub_loop);
    }
}

Server::~Server()
{
    delete m_acceptor;
    delete m_thpool;
}

void Server::NewConnection(Socket *sock)
{
    if (sock->GetFd() != -1)
    {
        int random = sock->GetFd() % m_subReactors.size();
        Connection *conn = new Connection(m_subReactors[random], sock);
        std::function<void(int)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
        conn->SetDeleteConnectionCallBack(cb);
        m_connections[sock->GetFd()] = conn;
    }
}

void Server::DeleteConnection(int sockfd)
{
    if (sockfd != -1)
    {
        auto it = m_connections.find(sockfd);
        if (it != m_connections.end())
        {
            Connection *conn = m_connections[sockfd];
            m_connections.erase(sockfd);
            // close(sockfd);
            delete conn;    // Segmant fault
        }
    }
}
