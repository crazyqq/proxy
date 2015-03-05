/*
 * worker.hpp
 *
 *  Created on: Aug 31, 2014
 *      Author: feasty
 */

#ifndef WORKER_HPP
#define WORKER_HPP

#include "threadpool.hpp"

class Worker
{
public:
    Worker(ThreadPool *s) : pool(s) { }

    void operator()();

private:
    ThreadPool *pool;
};

#endif // WORKER_HPP
