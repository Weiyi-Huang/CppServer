#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <cstdio>

Acceptor::Acceptor(EventLoop *loop) : m_loop(loop), m_sock(nullptr), m_acceptChannel(nullptr)
{
    m_sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1234);
    m_sock->Bind(addr);
    m_sock->Listen();
    m_sock->SetNonBlocking();

    m_acceptChannel = new Channel(m_loop, m_sock->GetFd());
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
    m_acceptChannel->SetCallback(cb);
    m_acceptChannel->EnableReading();

    delete addr;
}

Acceptor::~Acceptor()
{
    delete m_sock;
    delete m_acceptChannel;
}

void Acceptor::AcceptConnection()
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(m_sock->Accept(clnt_addr));
    clnt_sock->SetNonBlocking();

    printf("New client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr), ntohs(clnt_addr->GetAddr().sin_port));

    m_newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> cb)
{
    m_newConnectionCallback = cb;
}
