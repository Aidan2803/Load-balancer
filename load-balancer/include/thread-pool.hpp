#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    ThreadPool(int number_of_threads);

    template <typename F>
    void EnqueueTask(F task) {
        {
            std::unique_lock<std::mutex> ulock(queue_mutex_);
            tasks_.emplace(task);
        }
        condition_.notify_one();
    }

    ~ThreadPool();

  private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};
