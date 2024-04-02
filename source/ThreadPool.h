#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    void Add(std::function<void()> func);

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_taskmtx;
    std::condition_variable m_cv;
    bool m_stop;
};