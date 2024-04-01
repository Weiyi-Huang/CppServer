#pragma once

class Epoll;
class Channel;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void Loop();
    void UpdateChannel(Channel *ch);

private:
    Epoll *m_ep;
    bool m_quit;
};