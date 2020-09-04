#include "threadManager.h"

#include <assert.h>
#include <iterator>

using namespace std;

void Thread::processJobs()
{
    while(!shallStop)
    {
        std::unique_lock<std::mutex> lock(sleepMutex);
        sleepCondition.wait(lock, [this] {
           // [Goal]
           // Sleep Stop | r
           //     0    0   1
           //     0    1   1
           //     1    0   0
           //     1    1   1
           return this->shallStop || !this->shallSleep;
           // [Current State]
           // Sleep Stop | Stop !Sleep | f
           //     0    0 |    0      1 | 1
           //     0    1 |    1      1 | 1
           //     1    0 |    0      0 | 0
           //     1    1 |    1      0 | 1
        });

        if (shallStop)
        {
            continue;
        }

        Job job;

        if(jobs.size() > 0)
        {
            busyMutex.try_lock();

            isProcessing = true;

            // get job
            {
                lock_guard jobsLock(jobsMutex);
                job = std::move(jobs.front());
                jobs.erase(jobs.begin());
            }

            // process job
            int x = job();

            // store result
            {
                lock_guard resultsLock(resultsMutex);
                results.emplace_back(x);
            }

            isProcessing = false;
        }
        else 
        {
            busyMutex.unlock();
            busyCondition.notify_one();
        }
        lock.unlock();
    }
    busyMutex.unlock();
    busyCondition.notify_one();
}

ThreadManager::ThreadManager(int threadCount)
{
    threads.reserve(threadCount);
}

ThreadManager::~ThreadManager()
{
    for (std::shared_ptr<Thread> t : threads)
    {
        t->stop();
    }
}

vector<Result> ThreadManager::processJobs(vector<Job> &&jobs)
{

    const int THREAD_COUNT = threads.capacity();

    // Speicher für Ergebnisvektor anlegen
    this->finalResult.reserve(jobs.capacity());

    std::cout << "Starting threads..." << std::endl;
    createThreads(jobs.capacity() / THREAD_COUNT + 1);

    std::cout << "Assigning jobs...." << std::endl;
    for (const Job job : jobs) {
      addJob(Job(job));
    }

    std::cout << "Waking threads..." << std::endl;
    start();

    std::cout << "Waiting for all results..." << std::endl;
    stop();
    
    return move(finalResult);
}

void ThreadManager::addJob(Job &&job)
{
    static std::size_t current = 0;

    std::shared_ptr<Thread> currentThread = threads[current];
    currentThread->addJob(std::move(job));
    ++current;

    if (current >= threads.size())
    {
        current = 0;
    }
}

void ThreadManager::createThreads(std::size_t jobCount)
{
    assert(threads.size() < threads.capacity());

    for (std::size_t n = 0; n < threads.capacity(); ++n)
    {
        threads.emplace_back(new Thread(jobCount));
    }
}

void ThreadManager::start()
{
    for (std::shared_ptr<Thread> t : threads)
    {
        t->wakeUp();
    }
}

void ThreadManager::stop()
{
    for (std::shared_ptr<Thread> t : threads)
    {
        t->sleepWhenDone();
        vector<Result> r = t->takeResults();
        finalResult.insert(finalResult.end(), begin(r), end(r));
    }
}
