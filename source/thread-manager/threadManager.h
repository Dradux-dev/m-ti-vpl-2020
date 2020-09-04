#pragma once

#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <utility>
#include <queue>
#include <condition_variable>

/*
struct Job
{
	std::vector<float> imageData; 	// Bildinformationen
	std::vector<float> expectation; // erwarteter Ergebnisvektor
	enum Mode {training, test}; 	// Modusvorgabe
};
*/

struct Job
{
    int lhs;
    int rhs;
    inline Job(int lhs, int rhs)
        : lhs(lhs),
          rhs(rhs)
    {
    }
    Job() = default;
    Job(Job&&) = default;

    inline int operator()() 
    {
        return lhs + rhs;
    }
};

struct Result
{
    int result;
    inline explicit Result(int result)
        : result(result)
    {
    }
    Result() = default;
    Result(Result&&) = default;
};

class Thread : public std::thread
{
public:
    using lock_guard = std::lock_guard<std::mutex>;
    using unique_lock = std::unique_lock<std::mutex>;

    inline explicit Thread(std::size_t jobCount)
      : std::thread(&Thread::processJobs,this)
    {
        lock_guard jobsLock(jobsMutex);
        lock_guard resultsLock(resultsMutex);

        jobs.reserve(jobCount);
        results.reserve(jobCount);
    }

    virtual ~Thread()
    {
        join();
    }

    void processJobs();

    inline void addJob(Job&& job) 
    {
        lock_guard jobsLock(jobsMutex);
        jobs.push_back(std::move(job));
    }

    inline void addResult(Result&& result) 
    {
        lock_guard resultsLock(resultsMutex);
        results.push_back(std::move(result));
    }

    [[nodiscard]] inline bool isBusy() const noexcept
    {
        lock_guard jobsLock(jobsMutex);
        return jobs.size() || isProcessing;
    }

    inline void forceStop()
    {
        shallStop = true;
        sleepCondition.notify_one();
        join();
    }

    inline void stop()
    {
        waitForJobs();
        shallStop = true;
        sleepCondition.notify_one();
        join();
    }

    inline void sleepWhenDone()
    {
        waitForJobs();
        shallSleep = true;
    }

    inline void sleep()
    {
        shallSleep = true;
        /// @todo test if really required
        //sleepCondition.notify_one();
    }

    inline void wakeUp()
    {
        shallSleep = false;
        sleepCondition.notify_one();
    }

    inline void waitForJobs()
    {
        unique_lock lk(busyMutex);
        busyCondition.wait(lk, [this] { return !this->isBusy(); });
    }

private:
    mutable std::mutex jobsMutex;
    mutable std::mutex resultsMutex;
    std::queue<Job> jobs;
    std::queue<Result> results;
    volatile bool isProcessing = false;
    std::mutex sleepMutex;
    std::condition_variable sleepCondition;
    volatile bool shallSleep = true;
    std::mutex busyMutex;
    std::condition_variable busyCondition;
    volatile bool shallStop = false;
};

class ThreadManager
{
public:
    ThreadManager(int threadCount = 2);
    ~ThreadManager();
    std::vector<Result> processJobs(std::vector<Job> &&jobs);
    void addJob(Job &&job);
    void createThreads(std::size_t jobCount = 10);
    void start();
    void stop();

private:
    std::mutex m;
    std::vector<Thread> threads;
    std::vector<Result> finalResult;
};