#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    // void Add(std::function<void()> func);
    template <typename F, typename... Args>
    auto Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_taskmtx;
    std::condition_variable m_cv;
    bool m_stop;
};

template <typename F, typename... Args>
auto ThreadPool::Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_taskmtx);
        if (m_stop)
            throw std::runtime_error("ThreadPool already stop, can't add task any more!");
        m_tasks.emplace([task](){
            (*task)();
        });
    }
    m_cv.notify_one();
    return res;
}