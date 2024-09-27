#include "thread-pool.hpp"

#include <iostream>

ThreadPool::ThreadPool(int number_of_threads) {
    std::cout << "[ThreadPool] " << number_of_threads << " threads will be spawned\n";
    for (int i = 0; i < number_of_threads; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                std::cout << "[ThreadPool] in a while loop\n";
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> ulock(queue_mutex_);
                    std::cout << "[ThreadPool] after the ulock\n";
                    condition_.wait(ulock, [this] { return !tasks_.empty() || stop_; });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> ulock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();

    for (auto& t : threads_) {
        t.join();
    }
}