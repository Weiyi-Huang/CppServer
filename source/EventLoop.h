#pragma once
#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void Loop();
    void UpdateChannel(Channel *ch);
    
    void AddThread(std::function<void()> func);

private:
    Epoll *m_ep;
    ThreadPool *m_threadPool;
    bool m_quit;
};