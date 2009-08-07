/***************************************************************************
 *   Copyright (C) 2009 by Stefan Fuhrmann                                 *
 *   stefanfuhrmann@alice-dsl.de                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "stdafx.h"
#include "JobScheduler.h"
#include "IJob.h"
#include "Thread.h"

namespace async
{

// queue size management

void CJobScheduler::CQueue::Grow (size_t newSize)
{
    TJob* newData = new TJob[newSize];

    size_t count = size();
    memmove (newData, first, count * sizeof (TJob[1]));
    delete[] data;

    data = newData;
    end = newData + newSize;
    first = newData;
    last = newData + count;
}

void CJobScheduler::CQueue::AutoGrow()
{
    size_t count = size();

    if (count * 2 <= capacity())
    {
        memmove (data, first, count * sizeof (TJob[1]));
        first = data;
        last = data + count;
    }
    else
    {
        Grow (capacity() * 2);
    }
}

// remove one entry from \ref starving container.
// Return NULL, if container was empty

CJobScheduler* CJobScheduler::CThreadPool::SelectStarving()
{
    CCriticalSectionLock lock (mutex);
    if (starving.empty() || pool.empty())
        return NULL;

    std::vector<CJobScheduler*>::iterator begin = starving.begin();

    CJobScheduler* scheduler = *begin;
    starving.erase (begin);

    return scheduler;
}

// create empty thread pool

CJobScheduler::CThreadPool::CThreadPool()
{
}

// release threads

CJobScheduler::CThreadPool::~CThreadPool()
{
    SetThreadCount(0);
}

// Meyer's singleton

CJobScheduler::CThreadPool& CJobScheduler::CThreadPool::GetInstance()
{
    static CThreadPool instance;
    return instance;
}

// pool interface

CJobScheduler::SThreadInfo* CJobScheduler::CThreadPool::TryAlloc()
{
    CCriticalSectionLock lock (mutex);
    if (pool.empty())
        return NULL;

    CJobScheduler::SThreadInfo* thread = *pool.rbegin();
    pool.pop_back();
    ++allocCount;

    return thread;
}

void CJobScheduler::CThreadPool::Release (SThreadInfo* thread)
{
    {
        // put back into pool, unless its capacity has been exceeded

        CCriticalSectionLock lock (mutex);
        if (pool.size() + --allocCount < maxCount)
        {
            pool.push_back (thread);

            // shortcut

            if (starving.empty())
                return;
        }
        else
        {
            // pool overflow -> terminate thread

            delete thread->thread;
            delete thread;
        }
    }

    // notify starved schedulers that there may be some threads to alloc

    for ( CJobScheduler* scheduler = SelectStarving()
        ; scheduler != NULL
        ; scheduler = SelectStarving())
    {
        scheduler->ThreadAvailable();
    }
}

// set max. number of concurrent threads

void CJobScheduler::CThreadPool::SetThreadCount (size_t count)
{
    CCriticalSectionLock lock (mutex);

    maxCount = count;
    while (pool.size() + allocCount < maxCount)
    {
        SThreadInfo* info = new SThreadInfo;
        info->owner = NULL;
        info->thread = new CThread (&ThreadFunc, info, true);

        pool.push_back (info);
    }

    while ((pool.size() + allocCount > maxCount) && !pool.empty())
    {
        SThreadInfo* info = *pool.rbegin();
        pool.pop_back();

        delete info->thread;
        delete info;
    }
}

size_t CJobScheduler::CThreadPool::GetThreadCount()
{
    return maxCount;
}

// manage starving schedulers 

void CJobScheduler::CThreadPool::AddStarving (CJobScheduler* scheduler)
{
    CCriticalSectionLock lock (mutex);
    starving.push_back (scheduler);
}

bool CJobScheduler::CThreadPool::RemoveStarving (CJobScheduler* scheduler)
{
    CCriticalSectionLock lock (mutex);

    typedef std::vector<CJobScheduler*>::iterator TI;
    TI begin = starving.begin();
    TI end = starving.end();

    TI newEnd = std::remove_copy (begin, end, begin, scheduler);
    if (newEnd == end)
        return false;

    starving.erase (newEnd, end);
    return true;
}

// job execution helpers

CJobScheduler::TJob CJobScheduler::AssignJob (SThreadInfo* info)
{
    CCriticalSectionLock lock (mutex);

    // wake up threads that waited for some work to finish

    if (waitingThreads > 0)
        threadIsIdle.Set();

    // suspend this thread if there is no work left

    if (queue.empty())
    {
        // suspend

        info->thread->Suspend();

        // remove from "running" list and put it back either to
        // "suspended" pool or global shared pool

        for (size_t i = 0, count = threads.running.size(); i < count; ++i)
            if (threads.running[i] == info)
            {
                threads.running[i] = threads.running[count-1];
                threads.running.pop_back();

                // still in debt of shared pool?

                if (threads.fromShared > 0)
                {
                    // if we are actually idle, we aren't starving anymore

                    StopStarvation();

                    // put back to global pool

                    info->owner = NULL;
                    CThreadPool::GetInstance().Release (info);

                    --threads.fromShared;
                }
                else
                {
                    // add to local pool

                    threads.suspended.push_back (info);
                    ++threads.suspendedCount;
                }

                // signal empty queue, if necessary

                ++threads.unusedCount;
                if (--threads.runningCount == 0)
                    empty.Set();

                break;
            }

        return TJob (NULL, false);
    }

    // extract one job

    TJob result = queue.front();
    queue.pop();

    return result;
}

// try to get a thread from the shared pool.
// register as "starving" if that failed

bool CJobScheduler::AllocateSharedThread()
{
    if (!threads.starved)
    {
        SThreadInfo* info = CThreadPool::GetInstance().TryAlloc();
        if (info != NULL)
        {
            ++threads.fromShared;
            --threads.unusedCount;
            ++threads.runningCount;

            threads.running.push_back (info);
            info->owner = this;
            info->thread->Resume();

            return true;
        }
        else
        {
            threads.starved = true;
            CThreadPool::GetInstance().AddStarving (this);
        }
    }

    return false;
}

// unregister from "starving" list.
// This may race with CThreadPool::Release -> must loop here.

void CJobScheduler::StopStarvation()
{
    while (threads.starved)
        if (CThreadPool::GetInstance().RemoveStarving (this))
        {
            threads.starved = false;
            break;
        }
}

// worker thread function

void CJobScheduler::ThreadFunc (void* arg)
{
    SThreadInfo* info = reinterpret_cast<SThreadInfo*>(arg);

    TJob job = info->owner->AssignJob (info);
    if (job.first != NULL)
    {
        job.first->Execute();
        if (job.second)
            delete job.first;
    }
}

// Create & remove threads

CJobScheduler::CJobScheduler 
    ( size_t threadCount
    , size_t sharedThreads
    , bool aggressiveThreadStart)
    : waitingThreads (0)
    , aggressiveThreadStart (aggressiveThreadStart)
{
    threads.runningCount = 0;
    threads.suspendedCount = threadCount;

    threads.fromShared = 0;
    threads.maxFromShared = sharedThreads;

    threads.unusedCount = threadCount + sharedThreads;

    for (size_t i = 0; i < threadCount; ++i)
    {
        SThreadInfo* info = new SThreadInfo;
        info->owner = this;
        info->thread = new CThread (&ThreadFunc, info, true);

        threads.suspended.push_back (info);
    }

    threads.starved = false;

    // auto-initialize shared threads

    if (GetSharedThreadCount() == 0)
        UseAllCPUs();
}

CJobScheduler::~CJobScheduler(void)
{
    StopStarvation();

    WaitForEmptyQueue();

    assert (threads.running.empty());
    assert (threads.fromShared == 0);

    for (size_t i = 0, count = threads.suspended.size(); i < count; ++i)
    {
        SThreadInfo* info = threads.suspended[i];
        delete info->thread;
        delete info;
    }
}

// Meyer's singleton

CJobScheduler* CJobScheduler::GetDefault()
{
    static CJobScheduler instance (0, SIZE_MAX);
    return &instance;
}

// job management:
// add new job

void CJobScheduler::Schedule (IJob* job, bool transferOwnership)
{
    TJob toAdd (job, transferOwnership);

    CCriticalSectionLock lock (mutex);

    if (queue.empty())
        empty.Reset();

    queue.push (toAdd);

    bool addThread = aggressiveThreadStart
                 || (   (queue.size() > 2 * threads.runningCount)
                     && (threads.unusedCount > 0));

    if (addThread)
    {
        if (threads.suspendedCount > 0) 
        {
            SThreadInfo* info = *threads.suspended.rbegin();
            threads.suspended.pop_back();

            --threads.suspendedCount;
            --threads.unusedCount;
            ++threads.runningCount;

            threads.running.push_back (info);
            info->thread->Resume();
        }
        else 
        {
            if (threads.fromShared < threads.maxFromShared)
                AllocateSharedThread();
        }
    }
}

// notification that a new thread may be available

void CJobScheduler::ThreadAvailable()
{
    CCriticalSectionLock lock (mutex);
    threads.starved = false;

    while (   (queue.size() > 2 * threads.runningCount)
           && (threads.suspendedCount == 0)
           && (threads.fromShared < threads.maxFromShared))
    {
         if (!AllocateSharedThread())
            return;
    }
}

// wait for all current and follow-up jobs to terminate

void CJobScheduler::WaitForEmptyQueue()
{
    while (true)
    {
        {
            CCriticalSectionLock lock (mutex);
            if ((threads.runningCount == 0) && queue.empty())
                return;

            // we will be woken up as soon as both containers are emtpy

            empty.Reset();
        }

        empty.WaitFor();
    }
}

// wait for some jobs to be finished.

void CJobScheduler::WaitForSomeJobs()
{
    {
        CCriticalSectionLock lock (mutex);
        if (  static_cast<size_t>(InterlockedIncrement (&waitingThreads))
            < threads.runningCount)
        {
            // there are enough running job threads left
            // -> wait for one of them to run idle *or* 
            //    for too many of them to enter this method

            threadIsIdle.Reset();
        }
        else
        {
            // number of waiting jobs nor equal to or greater than
            // the number of running job threads
            // -> wake them all

            threadIsIdle.Set();
            InterlockedDecrement (&waitingThreads);

            return;
        }
    }

    threadIsIdle.WaitFor();
    InterlockedDecrement (&waitingThreads);
}

// set max. number of concurrent threads

void CJobScheduler::SetSharedThreadCount (size_t count)
{
    CThreadPool::GetInstance().SetThreadCount(count);
}

size_t CJobScheduler::GetSharedThreadCount()
{
    return CThreadPool::GetInstance().GetThreadCount();
}

void CJobScheduler::UseAllCPUs()
{
    SetSharedThreadCount (GetHWThreadCount());
}

size_t CJobScheduler::GetHWThreadCount()
{
#ifdef WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    
    size_t sysNumProcs = si.dwNumberOfProcessors;
#else
    size_t sysNumProcs = sysconf (_SC_NPROCESSORS_CONF);
#endif

    return sysNumProcs;
}

}