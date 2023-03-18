#include "include/ThreadPool.h"

#include <future>

ThreadPool::ThreadPool(int size) : stop(false) {
  for (int i = 0; i < size; ++i) {
    threads.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          // 该作用域内上锁，离开作用域自动解锁不需要unlock
          std::unique_lock<std::mutex> lock(tasks_mtx);
          // 线程池停止或者队列不为空进入临界区
          cv.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty())  // 线程池停止了线程池不为空直接退出线程。
            return;
          task = tasks.front();
          tasks.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    stop = true;
  }
  cv.notify_all();
  for (std::thread &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
}
