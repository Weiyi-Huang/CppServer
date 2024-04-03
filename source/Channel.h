#pragma once
#include <functional>

class Socket;
class EventLoop;
class Channel
{
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void HandEvent();
    void EnableRead();

    void UseET();
    int GetFd();
    uint32_t GetEvents();
    uint32_t GetReady();
    bool GetInEpoll();
    void SetInEpoll(bool in = true);

    void SetReady(uint32_t ev);
    void SetReadCallback(std::function<void()> cb);
    void SetUseThreadPool(bool use = true);

private:
    EventLoop *m_loop;
    int m_fd;
    uint32_t m_events;
    uint32_t m_ready;
    bool m_inEpoll;
    bool m_useThreadPool;
    std::function<void()> m_readCallback;
    std::function<void()> m_writeCallback;
};