#pragma once
#include <sys/epoll.h>

class Epoll;
class Channel
{
public:
    Channel(Epoll *ep, int fd);
    ~Channel();

    void EnableReading();

    int GetFd();
    uint32_t GetEvents();
    uint32_t GetRevents();
    bool GetInEpoll();
    void SetInEpoll();

    void SetRevents(uint32_t ev);

private:
    Epoll *m_ep;
    int m_fd;
    uint32_t m_events;
    uint32_t m_revents;
    bool m_inEpoll;
};