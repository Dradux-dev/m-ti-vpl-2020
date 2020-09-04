#include "threadManager.h"

using namespace std;

void Thread::processJobs()
{
    while(!shallStop)
    {
        std::unique_lock<std::mutex> lock(sleepMutex);
        sleepCondition.wait(lock, [shallSleep] {
            return !shallSleep && !shallStop;
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
                job = std::move(*jobs.front());
                jobs.pop();
            }

            // process job
            int x = job();

            // store result
            {
                lock_guard resultsLock(resultsMutex);
                results.emplace(x);
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
    for (Thread& t : threads)
    {
        t.stop();
    }
}

vector<Result> *ThreadManager::processJobs(vector<Job> *jobs)
{
    const int THREAD_COUNT = threads.capacity();

    // Speicher für Ergebnisvektor anlegen
    this->finalResult.reserve(jobs->capacity());

    // Anzahl der Elemente pro Thread ohne Rest
    int jobsPerThread = floor(jobs->size() / THREAD_COUNT);

    // Bestimmung des Restes für faire Verteilung über die anderen Threads
    int restJobs = jobs->size() % THREAD_COUNT;

    int fstElement = 0;
    int lastElement = jobsPerThread - 1;
    //auto fstElement = jobs->begin();
    //auto lastElement = jobs->begin() + jobsPerThread;

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        // Füge je ein Element zu den Threads hinzu bis der Rest gleichmäßg verteilt wurde
        if (restJobs > 0)
        {
            puts("Job angehängt");
            lastElement += 1;
            restJobs--;
        }

        printf("THREAD %d: Index %d - Index %d\n", i, fstElement, lastElement);
        threads.emplace_back(&ThreadManager::_processJobs, this, jobs, fstElement, lastElement);

        fstElement = lastElement + 1;
        lastElement = fstElement + jobsPerThread - 1;
    }

    for (thread &thread : threads)
    {
        thread.join();
    }
    
    return &finalResult;
}

void ThreadManager::addJob(Job &&job)
{
    static std::size_t current = 0;

    Thread &currentThread = threads[current];
    currentThread.addJob(job);
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
        threads.emplace_back(jobCount);
    }
}

void ThreadManager::start()
{
    for (Thread& t : threads)
    {
        t.wakeUp();
    }
}

void ThreadManager::stop()
{
    for (Thread& t : threads)
    {
        t.sleepWhenDone();
    }
}