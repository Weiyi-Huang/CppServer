#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>

Server::Server(EventLoop *loop) : m_loop(loop)
{
    m_acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    m_acceptor->SetNewConnectionCallback(cb);
}

Server::~Server()
{
    delete m_acceptor;
}

void Server::NewConnection(Socket *sock)
{
    Connection *conn = new Connection(m_loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
    conn->SetDeleteConnectionCallBack(cb);
    m_connections[sock->GetFd()] = conn;
}

void Server::DeleteConnection(Socket *sock)
{
    Connection *conn = m_connections[sock->GetFd()];
    m_connections.erase(sock->GetFd());
    delete conn;
}
