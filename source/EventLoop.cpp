#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : m_ep(nullptr), m_quit(false)
{
    m_ep = new Epoll();
    m_threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete m_threadPool;
    delete m_ep;
}

void EventLoop::Loop()
{
    while (!m_quit)
    {
        std::vector<Channel*> chs = m_ep->Poll();
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            (*it)->HandEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *ch)
{
    m_ep->UpdateChannel(ch);
}

void EventLoop::AddThread(std::function<void()> func)
{
    m_threadPool->Add(func);
}
