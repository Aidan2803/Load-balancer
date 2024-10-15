#include "thread-pool.hpp"

#include <iostream>

ThreadPool::ThreadPool(int number_of_threads) : instance_name_{"[ThreadPool]"} {
    spdlog::info("{} {} threads will be spawned", instance_name_, number_of_threads);

    for (int i = 0; i < number_of_threads; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> ulock(queue_mutex_);
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

int ThreadPool::GetCurrentTasksAmount() { return tasks_.size(); }

int ThreadPool::GetMaxThreadsAmount() { return threads_.size(); }

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