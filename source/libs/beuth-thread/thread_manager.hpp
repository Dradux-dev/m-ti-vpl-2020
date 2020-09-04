#pragma once

#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <utility>
#include <queue>
#include <condition_variable>
#include <assert.h>
#include <iterator>

#include "thread.hpp"

namespace beuth {
  namespace thread {
    template <typename TJob, typename TResult>
    class ThreadManager
    {
    public:
        using Job = TJob;
        using Result = TResult;
        using Thread = Thread<Job,Result>;
        using Process = std::function<Result(Job&&)>;

        ThreadManager(int threadCount, Process process);
        ~ThreadManager();
        std::vector<Result> processJobs(std::vector<Job> &&jobs);
        void addJob(Job &&job);
        void createThreads(std::size_t jobCount);
        void start();
        void stop();

    private:
        Process process;
        std::mutex m;
        std::vector<std::shared_ptr<Thread>> threads;
        std::vector<Result> finalResult;
    };

    template <typename TJob, typename TResult>
    ThreadManager<TJob,TResult>::ThreadManager(int threadCount, Process process)
      : process(process)
    {
        threads.reserve(threadCount);
    }

    template <typename TJob, typename TResult>
    ThreadManager<TJob,TResult>::~ThreadManager()
    {
        for (std::shared_ptr<Thread> t : threads)
        {
            t->stop();
        }
    }

    template <typename TJob, typename TResult>
    std::vector<typename ThreadManager<TJob,TResult>::Result> ThreadManager<TJob,TResult>::processJobs(std::vector<Job> &&jobs)
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

    template <typename TJob, typename TResult>
    void ThreadManager<TJob,TResult>::addJob(Job &&job)
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

    template <typename TJob, typename TResult>
    void ThreadManager<TJob,TResult>::createThreads(std::size_t jobCount)
    {
        assert(threads.size() < threads.capacity());

        for (std::size_t n = 0; n < threads.capacity(); ++n)
        {
            threads.emplace_back(new Thread(jobCount, process));
        }
    }

    template <typename TJob, typename TResult>
    void ThreadManager<TJob,TResult>::start()
    {
        for (std::shared_ptr<Thread> t : threads)
        {
            t->wakeUp();
        }
    }

    template <typename TJob, typename TResult>
    void ThreadManager<TJob,TResult>::stop()
    {
        for (std::shared_ptr<Thread> t : threads)
        {
            t->sleepWhenDone();
            std::vector<Result> r = t->takeResults();
            finalResult.insert(finalResult.end(), std::begin(r), std::end(r));
        }
    }
  }
}
