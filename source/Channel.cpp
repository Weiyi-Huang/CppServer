#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int fd) : m_loop(loop), m_fd(fd), m_events(0), m_ready(0),
                                            m_inEpoll(false)
{
}

Channel::~Channel()
{
    if (m_fd != -1)
    {
        close(m_fd);
        m_fd = -1;
    }
}

void Channel::HandEvent()
{
    if (m_ready & (EPOLLIN | EPOLLPRI))
    {
        m_readCallback();
    }
    if (m_ready & (EPOLLOUT))
    {
        m_writeCallback();
    }
}

void Channel::EnableRead()
{
    m_events = EPOLLIN | EPOLLPRI;
    m_loop->UpdateChannel(this);
}

void Channel::UseET()
{
    m_events |= EPOLLET;
    m_loop->UpdateChannel(this);
}

int Channel::GetFd()
{
    return m_fd;
}

uint32_t Channel::GetEvents()
{
    return m_events;
}

uint32_t Channel::GetReady()
{
    return m_ready;
}

bool Channel::GetInEpoll()
{
    return m_inEpoll;
}

void Channel::SetInEpoll(bool in)
{
    m_inEpoll = in;
}

void Channel::SetReady(uint32_t ev)
{
    m_ready = ev;
}

void Channel::SetReadCallback(std::function<void()> cb)
{
    m_readCallback = cb;
}