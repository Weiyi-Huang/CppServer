#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();

    void UpdateChannel(Channel *channel);
    void DeleteChannel(Channel *channel);
    std::vector<Channel *> Poll(int timeout = -1);

private:
    int m_epfd;
    epoll_event *m_events;
};