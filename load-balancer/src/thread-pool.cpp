#include "thread-pool.hpp"

ThreadPool::ThreadPool(int number_of_threads) {
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