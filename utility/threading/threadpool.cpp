
#include "threadpool.hpp"
#include "worker.hpp"

using namespace std;

// The constructor
ThreadPool::ThreadPool(size_t num_threads)
    : stop(false)
{
    for(size_t i = 0; i < num_threads; ++i)
    {
        workers.push_back(thread(Worker(this)));
    }
}

// The destructor
ThreadPool::~ThreadPool()
{
    // Lock the threads to ensure all threads receive the
    // stop notification.
    {
        unique_lock<mutex> lock(queue_mutex);

        // Stop all threads
        stop = true;
        condition.notify_all();
    } // Release lock

    // Join all threads
    for(size_t i = 0; i < workers.size(); ++i)
    {
        workers[i].join();
    }
}


// Adds work to the work queue.
void ThreadPool::add_work(std::function<void()> work)
{
    { // Acquire lock
        unique_lock<mutex> lock(queue_mutex);

        // Add the task
        tasks.push_back(work);
    } // Release lock

    // Wake up one thread
    condition.notify_one();
}
