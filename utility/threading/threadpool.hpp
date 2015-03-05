#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

#include "base/types.hpp"

class ThreadPool
{
public:
    //
    // @brief   The constructor which launches num_threads.
    //
    // @param[in]   num_threads The number of threads to launch in the pool.
    //
    ThreadPool(size_t num_threads);

    //
    // @brief   The destructor
    //
    virtual ~ThreadPool();

    //
    // @brief   Adds work to the work queue.
    //
    // @param[in]   work    Function pointer to the work that needs to be processed.
    //
    void add_work(std::function<void()> work);

private:
    friend class Worker;

    //
    // @var     workers
    //
    //  @brief  Vector of threads to keep track of all threads in the pool.
    //
    std::vector< std::thread > workers;

    //
    // @var     tasks
    //
    // @brief   A task queue to add work to.
    //
    std::deque< std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif // THREAD_POOL_HPP
