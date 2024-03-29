/*
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */


#include <sys/prctl.h>
#include <functional>
#include <iostream>
#include "dispatchqueue.h"

DispatchQueue::DispatchQueue(std::string name, size_t threadCount) :
    name{std::move(name)}, threads(threadCount) {
    for(size_t i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread(&DispatchQueue::dispatchThreadHandler, this);
    }
}

DispatchQueue::~DispatchQueue() {
    // Signal to dispatch threads that it's time to wrap up
    std::unique_lock<std::mutex> lock(lockMutex);
    quit = true;
    lock.unlock();
    condition.notify_all();

    // Wait for threads to finish before we exit
    for(size_t i = 0; i < threads.size(); i++)
    {
        if(threads[i].joinable())
        {
            threads[i].join();
        }
    }
}

void DispatchQueue::removePending() {
    std::unique_lock<std::mutex> lock(lockMutex);
    queue = {};
}

void DispatchQueue::dispatch(const fp_t& op) {
    std::unique_lock<std::mutex> lock(lockMutex);
    queue.push(op);

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    condition.notify_one();
}

void DispatchQueue::dispatch(fp_t&& op) {
    std::unique_lock<std::mutex> lock(lockMutex);
    queue.push(std::move(op));

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    condition.notify_one();
}

void DispatchQueue::dispatchThreadHandler(void) {
    prctl(PR_SET_NAME, name.c_str());
    std::unique_lock<std::mutex> lock(lockMutex);
    // we got lock if come here

    do {
        condition.wait(lock, [this]{
            // do nothing if here return true. do wait(lock) if here return false
            // release lock and wait for signal at the point of enter wait(lock)
            // return from wait(lock) when got signal and lock succeed
            return (queue.size() || quit);
        });

        //after wait, we own the lock
        if(!quit && queue.size())
        {
            auto op = std::move(queue.front());
            queue.pop();

            //unlock now that we're done messing with the queue
            lock.unlock();

            op();

            lock.lock();
        }
    } while (!quit);
    // ~std:unique_lock() will do unlock if owns lock
}
