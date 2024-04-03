#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : m_ep(nullptr), m_quit(false)
{
    m_ep = new Epoll();
}

EventLoop::~EventLoop()
{
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