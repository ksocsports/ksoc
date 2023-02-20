// Copyright (c) 2015 The Bitcoin Core developers
// Copyright (c) 2017-2019 The KSOC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "scheduler.h"

#include "random.h"

#include <assert.h>
#include <boost/bind.hpp>
#include <utility>

CScheduler::CScheduler() : nThreadsServicingQueue(0), stopRequested(false), stopWhenEmpty(false)
{
}

CScheduler::~CScheduler()
{
    assert(nThreadsServicingQueue == 0);
}

void CScheduler::serviceQueue()
{
    WAIT_LOCK(newTaskMutex, lock);
    ++nThreadsServicingQueue;

    // newTaskMutex is locked throughout this loop EXCEPT
    // when the thread is waiting or when the user's function
    // is called.
    while (!shouldStop()) {
        try {
            if (!shouldStop() && taskQueue.empty()) {
                REVERSE_LOCK(lock);
                // Use this chance to get a tiny bit more entropy
                RandAddSeedSleep();
            }
            while (!shouldStop() && taskQueue.empty()) {
                // Wait until there is something to do.
                newTaskScheduled.wait(lock);
            }

            // Wait until either there is a new task, or until
            // the time of the first item on the queue:

            // Some boost versions have a conflicting overload of wait_until that returns void.
            // Explicitly use a template here to avoid hitting that overload.
            while (!shouldStop() && !taskQueue.empty()) {
                std::chrono::system_clock::time_point timeToWaitFor = taskQueue.begin()->first;
                if (newTaskScheduled.wait_until(lock, timeToWaitFor) == std::cv_status::timeout) {
                    break; // Exit loop after timeout, it means we reached the time of the event
                }
            }

            Function f = taskQueue.begin()->second;
            taskQueue.erase(taskQueue.begin());

            {
                // Unlock before calling f, so it can reschedule itself or another task
                // without deadlocking:
                REVERSE_LOCK(lock);
                f();
            }
        } catch (...) {
            --nThreadsServicingQueue;
            throw;
        }
    }
    --nThreadsServicingQueue;
    newTaskScheduled.notify_one();
}

void CScheduler::stop(bool drain)
{
    {
        LOCK(newTaskMutex);
        if (drain)
            stopWhenEmpty = true;
        else
            stopRequested = true;
    }
    newTaskScheduled.notify_all();
}

void CScheduler::schedule(CScheduler::Function f, std::chrono::system_clock::time_point t)
{
    {
        LOCK(newTaskMutex);
        taskQueue.insert(std::make_pair(t, f));
    }
    newTaskScheduled.notify_one();
}

void CScheduler::scheduleFromNow(CScheduler::Function f, int64_t deltaSeconds)
{
    schedule(f, std::chrono::system_clock::now() + std::chrono::seconds(deltaSeconds));
}

static void Repeat(CScheduler* s, CScheduler::Function f, int64_t deltaSeconds)
{
    f();
    s->scheduleFromNow(boost::bind(&Repeat, s, f, deltaSeconds), deltaSeconds);
}

void CScheduler::scheduleEvery(CScheduler::Function f, int64_t deltaSeconds)
{
    scheduleFromNow(boost::bind(&Repeat, this, f, deltaSeconds), deltaSeconds);
}

size_t CScheduler::getQueueInfo(std::chrono::system_clock::time_point &first,
                             std::chrono::system_clock::time_point &last) const
{
    LOCK(newTaskMutex);
    size_t result = taskQueue.size();
    if (!taskQueue.empty()) {
        first = taskQueue.begin()->first;
        last = taskQueue.rbegin()->first;
    }
    return result;
}
