#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <unistd.h>
#include <functional>

Server::Server(EventLoop *loop) : m_loop(loop), m_acceptor(nullptr)
{
    m_acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    m_acceptor->SetNewConnectionCallback(cb);
}

Server::~Server()
{
    delete m_acceptor;
}

void Server::NewConnection(Socket *sock)
{
    if (sock->GetFd() != -1)
    {
        Connection *conn = new Connection(m_loop, sock);
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
            close(sockfd);
            // delete conn;    // Segmant fault
        }
    }
}
