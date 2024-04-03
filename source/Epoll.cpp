#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define MAX_EVENTS 1000
// #define DEBUG

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

void Epoll::DeleteChannel(Channel *channel)
{
    int fd = channel->GetFd();
    ErrorIf(epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL) == -1, "Epoll delete error!");
    channel->SetInEpoll(false);
}

std::vector<Channel*> Epoll::Poll(int timeout)
{
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
    ErrorIf(nfds == -1, "Epoll wait error!");
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel*)m_events[i].data.ptr;
        ch->SetReady(m_events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}
