#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *loop) : m_loop(loop)
{
    m_sock = new Socket();
    m_addr = new InetAddress("127.0.0.1", 8888);
    m_sock->Bind(m_addr);
    m_sock->Listen();
    m_sock->SetNonBlocking();

    m_acceptChannel = new Channel(m_loop, m_sock->GetFd());
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
    m_acceptChannel->SetCallback(cb);
    m_acceptChannel->EnableReading();
}

Acceptor::~Acceptor()
{
    delete m_sock;
    delete m_addr;
    delete m_acceptChannel;
}

void Acceptor::AcceptConnection()
{
    m_newConnectionCallback(m_sock);
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> cb)
{
    m_newConnectionCallback = cb;
}
