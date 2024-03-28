#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *ep, int fd) : m_ep(ep), m_fd(fd), m_events(0), m_revents(0), m_inEpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::EnableReading()
{
    m_events = EPOLLIN | EPOLLET;
    m_ep->UpdateChannel(this);
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
