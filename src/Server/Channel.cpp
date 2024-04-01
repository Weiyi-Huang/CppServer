#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd) : m_loop(loop), m_fd(fd), m_events(0), m_revents(0), m_inEpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::HandEvent()
{
    m_callback();
}

void Channel::EnableReading()
{
    m_events = EPOLLIN | EPOLLET;
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

uint32_t Channel::GetRevents()
{
    return m_revents;
}

bool Channel::GetInEpoll()
{
    return m_inEpoll;
}

void Channel::SetInEpoll()
{
    m_inEpoll = true;
}

void Channel::SetRevents(uint32_t ev)
{
    m_revents = ev;
}

void Channel::SetCallback(std::function<void()> cb)
{
    m_callback = cb;
}