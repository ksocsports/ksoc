// Copyright (c) 2015 The Bitcoin Core developers
// Copyright (c) 2017-2019 The KSOC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SCHEDULER_H
#define BITCOIN_SCHEDULER_H

//
// NOTE:
// boost::thread / boost::chrono should be ported to std::thread / std::chrono
// when we support C++11.
//
#include <condition_variable>
#include <functional>
#include <list>
#include <map>

//
// Simple class for background tasks that should be run
// periodically or once "after a while"
//
// Usage:
//
// CScheduler* s = new CScheduler();
// s->scheduleFromNow(doSomething, 11); // Assuming a: void doSomething() { }
// s->scheduleFromNow(boost::bind(Class::func, this, argument), 3);
// boost::thread* t = new boost::thread(boost::bind(CScheduler::serviceQueue, s));
//
// ... then at program shutdown, clean up the thread running serviceQueue:
// t->interrupt();
// t->join();
// delete t;
// delete s; // Must be done after thread is interrupted/joined.
//

class CScheduler
{
public:
    CScheduler();
    ~CScheduler();

    typedef std::function<void(void)> Function;

    // Call func at/after time t
    void schedule(Function f, std::chrono::system_clock::time_point t);

    // Convenience method: call f once deltaSeconds from now
    void scheduleFromNow(Function f, int64_t deltaSeconds);

    // Another convenience method: call f approximately
    // every deltaSeconds forever, starting deltaSeconds from now.
    // To be more precise: every time f is finished, it
    // is rescheduled to run deltaSeconds later. If you
    // need more accurate scheduling, don't use this method.
    void scheduleEvery(Function f, int64_t deltaSeconds);

    // To keep things as simple as possible, there is no unschedule.

    // Services the queue 'forever'. Should be run in a thread,
    // and interrupted using boost::interrupt_thread
    void serviceQueue();

    // Tell any threads running serviceQueue to stop as soon as they're
    // done servicing whatever task they're currently servicing (drain=false)
    // or when there is no work left to be done (drain=true)
    void stop(bool drain=false);

    // Returns number of tasks waiting to be serviced,
    // and first and last task times
    size_t getQueueInfo(std::chrono::system_clock::time_point &first,
                        std::chrono::system_clock::time_point &last) const;

private:
mutable Mutex newTaskMutex;
    std::condition_variable newTaskScheduled;
    std::multimap<std::chrono::system_clock::time_point, Function> taskQueue GUARDED_BY(newTaskMutex);
    int nThreadsServicingQueue GUARDED_BY(newTaskMutex);
    bool stopRequested GUARDED_BY(newTaskMutex);
    bool stopWhenEmpty GUARDED_BY(newTaskMutex);
    bool shouldStop() const EXCLUSIVE_LOCKS_REQUIRED(newTaskMutex) { return stopRequested || (stopWhenEmpty && taskQueue.empty()); }
};

#endif
