#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                {
                    // 该作用域内上锁，离开作用域自动解锁不需要unlock
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this]() {
                        return stop || !tasks.empty();  // 线程池停止或者队列不为空进入临界区
                    });
                    if (stop && tasks.empty())  // 线程池停止了线程池不为空直接退出线程。
                        return ;
                    task = tasks.front();
                    task();
                }
            }
        }));
    }
}

void ThreadPool::add(std::function<void()> func) {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if (stop) {
            throw std::runtime_error("ThreadPool already stop, can't add task any more!");
        }
        tasks.emplace(func);
    }
    cv.notify_one();
}