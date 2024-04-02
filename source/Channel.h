#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void HandEvent();
    void EnableReading();

    int GetFd();
    uint32_t GetEvents();
    uint32_t GetRevents();
    bool GetInEpoll();
    void SetInEpoll();

    void SetRevents(uint32_t ev);
    void SetCallback(std::function<void()> cb);

private:
    EventLoop *m_loop;
    int m_fd;
    uint32_t m_events;
    uint32_t m_revents;
    bool m_inEpoll;
    std::function<void()> m_callback;
};