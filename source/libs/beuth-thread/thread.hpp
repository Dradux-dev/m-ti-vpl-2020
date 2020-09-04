#pragma once

#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <utility>
#include <queue>
#include <condition_variable>
#include <functional>

namespace beuth {
  namespace thread {
    template <typename TJob, typename TResult>
    class Thread : public std::thread
    {
      public:
        using Job = TJob;
        using Result = TResult;
        using lock_guard = std::lock_guard<std::mutex>;
        using unique_lock = std::unique_lock<std::mutex>;
        using Process = std::function<Result(Job&&)>;

        inline explicit Thread(std::size_t jobCount, Process process);
        virtual ~Thread();

        inline void addJob(Job&& job);

        inline void wakeUp();
        inline void waitForJobs();
        inline void sleep();
        inline void sleepWhenDone();
        inline void forceStop();
        inline void stop();

        [[nodiscard]] inline std::vector<Result> takeResults() noexcept;

        [[nodiscard]] inline bool isBusy() const noexcept;

      protected:
        void processJobs();
        inline void addResult(Result&& result);

      private:
        Process process;
        mutable std::mutex jobsMutex;
        mutable std::mutex resultsMutex;
        std::vector<Result> results;
        std::vector<Job> jobs;
        volatile bool isProcessing = false;
        std::mutex sleepMutex;
        std::condition_variable sleepCondition;
        volatile bool shallSleep = true;
        std::mutex busyMutex;
        std::condition_variable busyCondition;
        volatile bool shallStop = false;
    };

    template <typename TJob, typename TResult>
    inline Thread<TJob,TResult>::Thread(std::size_t jobCount, Process process)
      : std::thread(&Thread<TJob,TResult>::processJobs,this),
        process(process)
    {
        lock_guard jobsLock(jobsMutex);
        lock_guard resultsLock(resultsMutex);

        jobs.reserve(jobCount);
        results.reserve(jobCount);
    }

    template <typename TJob, typename TResult>
    Thread<TJob,TResult>::~Thread()
    {
        if (joinable()) {
            join();
        }
    }

    template <typename TJob, typename TResult>
    void Thread<TJob,TResult>::processJobs()
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
                Result result = process(std::move(job));

                // store result
                {
                    lock_guard resultsLock(resultsMutex);
                    results.push_back(std::move(result));
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

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::addJob(Job&& job)
    {
        lock_guard jobsLock(jobsMutex);
        jobs.push_back(std::move(job));
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::addResult(Result&& result)
    {
        lock_guard resultsLock(resultsMutex);
        results.push_back(std::move(result));
    }

    template <typename TJob, typename TResult>
    [[nodiscard]] inline bool Thread<TJob,TResult>::isBusy() const noexcept
    {
        lock_guard jobsLock(jobsMutex);
        return jobs.size() || isProcessing;
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::forceStop()
    {
        shallStop = true;
        sleepCondition.notify_one();
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::stop()
    {
        waitForJobs();
        shallStop = true;
        sleepCondition.notify_one();
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::sleepWhenDone()
    {
        waitForJobs();
        shallSleep = true;
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::sleep()
    {
        shallSleep = true;
        /// @todo test if really required
        //sleepCondition.notify_one();
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::wakeUp()
    {
        shallSleep = false;
        sleepCondition.notify_one();
    }

    template <typename TJob, typename TResult>
    inline void Thread<TJob,TResult>::waitForJobs()
    {
        unique_lock lk(busyMutex);
        busyCondition.wait(lk, [this] { return !this->isBusy(); });
    }

    template <typename TJob, typename TResult>
    [[nodiscard]] inline std::vector<typename Thread<TJob,TResult>::Result> Thread<TJob,TResult>::takeResults() noexcept
    {
        lock_guard resultsLock(resultsMutex);
        return std::move(results);
    }
  }
}


