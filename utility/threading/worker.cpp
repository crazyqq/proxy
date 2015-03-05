/*
 * worker.cpp
 *
 *  Created on: Aug 31, 2014
 *      Author: feasty
 */
#include <thread>
#include <mutex>
#include <condition_variable>

#include "worker.hpp"

#include <functional>

using namespace std;

void Worker::operator()()
{
    function<void()> task;

    while(true)
    {
        {   // acquire lock
            unique_lock<mutex> lock(pool->queue_mutex);

            // Look for a work item
            while(!pool->stop && pool->tasks.empty())
            {
                pool->condition.wait(lock);
            }

            if(pool->stop)
            {
                return;
            }

            // Get the task from the queue
            task = pool->tasks.front();
            pool->tasks.pop_front();

        }   // release lock

        // Execute the task
        task();
    }
}
