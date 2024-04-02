#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define MAX_EVENTS 1000
#define DEBUG

Epoll::Epoll() : m_epfd(-1), m_events(nullptr)
{
    m_epfd = epoll_create1(0);
    ErrorIf(m_epfd == -1, "Epoll create error!");
    m_events = new epoll_event[MAX_EVENTS];
    bzero(m_events, sizeof(*m_events) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (m_epfd == -1)
    {
        close(m_epfd);
        m_epfd = -1;
    }
    delete[] m_events;
}

// void Epoll::AddFd(int fd, uint32_t op)
// {
//     epoll_event ev;
//     bzero(&ev, sizeof(ev));
//     ev.data.fd = fd;
//     ev.events = op;
//     ErrorIf(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Epoll add event error!");
// }

void Epoll::UpdateChannel(Channel *channel)
{
    int fd = channel->GetFd();
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->GetEvents();
    if (!channel->GetInEpoll())
    {
        ErrorIf(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Epoll add error!");
        channel->SetInEpoll();
#ifdef DEBUG
        printf("Epoll: add Channel to epoll tree success, the Channel's fd is: %d\n", fd);
#endif
    }
    else
    {
        ErrorIf(epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "Epoll modify error!");
#ifdef DEBUG
        printf("Epoll: modify Channel to epoll tree success, the Channel's fd is: %d\n", fd);
#endif
    }
}

// std::vector<epoll_event> Epoll::Poll(int timeout)
// {
//     std::vector<epoll_event> activeEvents;
//     int nfds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
//     ErrorIf(nfds == -1, "Epoll wait error!");
//     for (int i = 0; i < nfds; ++i)
//     {
//         activeEvents.push_back(m_events[i]);
//     }
//     return activeEvents;
// }

std::vector<Channel*> Epoll::Poll(int timeout)
{
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
    ErrorIf(nfds == -1, "Epoll wait error!");
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel*)m_events[i].data.ptr;
        ch->SetRevents(m_events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}
