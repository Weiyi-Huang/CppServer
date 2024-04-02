#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : m_stop(false)
{
    for (int i = 0; i < size; ++i)
    {
        m_threads.emplace_back(std::thread([this](){
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_taskmtx);
                    m_cv.wait(lock, [this](){
                        return m_stop || !m_tasks.empty();
                    });
                    if (m_stop && m_tasks.empty())
                        return;
                    task = m_tasks.front();
                    m_tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_taskmtx);
        m_stop = true;
    }
    m_cv.notify_all();
    for (std::thread &th : m_threads)
    {
        if (th.joinable())
            th.join();
    }
}

void ThreadPool::Add(std::function<void()> func)
{
    {
        std::unique_lock<std::mutex> lock(m_taskmtx);
        if (m_stop)
            throw std::runtime_error("ThreadPool already stop, can't add task any more!");
        m_tasks.emplace(func);
    }
    m_cv.notify_one();
}
