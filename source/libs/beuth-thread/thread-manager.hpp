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

#include "batch.hpp"
#include "threadpool.h"

namespace beuth {
  namespace thread {
    /**
     * @brief Thread manager
     *
     * Provides an interface to a thread pool for assigning jobs that generate results to it.
     *
     * @tparam TJob The job type.
     * @tparam TResult The result type.
     *
     * @author Christian Haß
     */
    template <typename TJob, typename TResult>
    class ThreadManager {
      public:
        /// Job type definition
        using Job = TJob;
        /// Result type definition
        using Result = TResult;
        /// Function signature to perform a job to get the result
        using Process = std::function<Result(Job&&)>;

        /**
         * @brief Constructor
         *
         * Constructs a manager based on a thread pool and a given operation to
         * perfom a job and aquire the corresponding result.
         *
         * @param pool The thread pool that shall be used.
         * @param process The process that is able to perform a job and generated
         *                the correspondig result.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool, Process process);

        /**
         * @brief Constructor
         *
         * Constructs a manager based on a thread pool and a given operation to
         * perfom a job and aquire the corresponding result. As well as the full
         * list ob jobs that need to be performed.
         *
         * @param pool The thread pool that shall be used.
         * @param process The process that is able to perform a job and generated
         *                the correspondig result.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process proccess);

        /**
         * @brief reserve
         *
         * Reserves memory for the amount of jobs to be stored in.
         *
         * @param amount Amount of jobs that needs to be stored.
         *
         * @author Christian Haß
         */
        inline void reserve(std::size_t amount);

        /**
         * @brief add job
         *
         * Adds a job the job list.
         *
         * @param job The job that shall be added to the list.
         *
         * @author Christian Haß
         */
        inline void addJob(Job&& job);

        /**
         * @brief execute
         *
         * Executes all jobs concurrently and returns the results
         * of those jobs.
         *
         * @return Returns the result of the jobs.
         *
         * @attention The order of result may differ from the order of the
         *            assigned jobs. This depends on the execution speed of
         *            each single job as well as the cpu time availability
         *            per thread. It may be required to add an order value
         *            to the job and the result to be able to sort the results
         *            after the execution.
         *
         * @author Christian Haß
         */
        inline std::vector<Result> execute();

      protected:
        /// Reference to the used thread pool
        Threadpool& pool;

        /// List of assigned jobs
        std::vector<Job> jobs;

        /// Function to perform a job and generate the corresponding result.
        Process process;
    };

    template <typename TJob, typename TResult>
    inline ThreadManager<TJob,TResult>::ThreadManager(Threadpool& pool, Process process)
      : pool(pool),
        process(process)
    {}

    template <typename TJob, typename TResult>
    inline ThreadManager<TJob,TResult>::ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process process)
      : pool(pool),
        jobs(std::move(jobs)),
        process(process)
    {}

    template <typename TJob, typename TResult>
    inline void ThreadManager<TJob,TResult>::reserve(std::size_t amount) {
      jobs.reserve(amount);
    }

    template <typename TJob, typename TResult>
    inline void ThreadManager<TJob,TResult>::addJob(Job&& job) {
      jobs.emplace_back(std::move(job));
    }

    template <typename TJob, typename TResult>
    inline std::vector<typename ThreadManager<TJob, TResult>::Result> ThreadManager<TJob, TResult>::execute() {
      return std::move(batch<TJob, TResult>(pool, std::move(jobs), process));
    }


    /**
     * @brief Thread manager
     *
     * Provides an interface to a thread pool for assigning jobs to it.
     *
     * @tparam TJob The job type.
     *
     * @author Christian Haß
     */
    template <typename TJob>
    class ThreadManager<TJob, void> {
      public:
        /// Job type definition
        using Job = TJob;
        /// Function signature to perform a job
        using Process = std::function<void(Job&&)>;

        /**
         * @brief Constructor
         *
         * Initializes the manager with a thread pool to use and a function to perform a job.
         *
         * @param pool The thread pool that shall be used.
         * @param process The function that can perform a job.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool, Process process);

        /**
         * @brief Thread manager
         *
         * Initializes the manager with a thread pool, a list of jobs to perform as well as a
         * function that is able to perform the jobs.
         *
         * @param pool The thread pool that shall be used.
         * @param jobs List of jobs that shall be performed.
         * @param proccess Function that is able to perform a job.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process proccess);

        /**
         * @brief reserve
         *
         * Reserves memory for the amount of jobs to be stored in.
         *
         * @param amount Amount of jobs that needs to be stored.
         *
         * @author Christian Haß
         */
        inline void reserve(std::size_t amount);

        /**
         * @brief add job
         *
         * Adds a job the job list.
         *
         * @param job The job that shall be added to the list.
         *
         * @author Christian Haß
         */
        inline void addJob(Job&& job);

        /**
         * @brief execute
         *
         * Executes all jobs concurrently and returns the results
         * of those jobs.
         *
         * @return Returns the result of the jobs.
         *
         * @attention The order of result may differ from the order of the
         *            assigned jobs. This depends on the execution speed of
         *            each single job as well as the cpu time availability
         *            per thread. It may be required to add an order value
         *            to the job and the result to be able to sort the results
         *            after the execution.
         *
         * @author Christian Haß
         */
        inline void execute();

      protected:
        /// Reference to the used thread pool
        Threadpool& pool;

        /// List of jobs that need to be performed
        std::vector<Job> jobs;

        /// Function that is able to perform a single job
        Process process;
    };

    template <typename TJob>
    inline ThreadManager<TJob,void>::ThreadManager(Threadpool& pool, Process process)
      : pool(pool),
        process(process)
    {}

    template <typename TJob>
    inline ThreadManager<TJob,void>::ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process process)
      : pool(pool),
        jobs(std::move(jobs)),
        process(process)
    {}

    template <typename TJob>
    inline void ThreadManager<TJob,void>::reserve(std::size_t amount) {
      jobs.reserve(amount);
    }

    template <typename TJob>
    inline void ThreadManager<TJob,void>::addJob(Job&& job) {
      jobs.emplace_back(std::move(job));
    }

    template <typename TJob>
    inline void ThreadManager<TJob, void>::execute() {
      return batch<TJob>(pool, std::move(jobs), process);
    }

    /**
     * @brief Thread manager
     *
     * Provides an interface to a thread pool for performing jobs concurrently.
     *
     * @author Christian Haß
     */
    template <>
    class ThreadManager<void, void> {
      public:
        /// Function signature to perform specific task
        using Process = Threadpool::Process;

        /**
         * @brief Constructor
         *
         * Initializes the manager with a thread pool, that shall be used.
         *
         * @param pool The thread pool that shall be used.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool);

        /**
         * @brief Constructor
         *
         * Initializes the manager with a thread pool to use and a list of
         * jobs, that shall be performed concurrently.
         *
         * @param pool The thread pool that shall be used.
         * @param jobs List of jobs that need to be performed.
         *
         * @author Christian Haß
         */
        explicit inline ThreadManager(Threadpool& pool, std::vector<Process>&& jobs);

        /**
         * @brief reserve
         *
         * Reserves memory for the amount of jobs to be stored in.
         *
         * @param amount Amount of jobs that needs to be stored.
         *
         * @author Christian Haß
         */
        inline void reserve(std::size_t amount);

        /**
         * @brief add job
         *
         * Adds a job the job list.
         *
         * @param job The job that shall be added to the list.
         *
         * @author Christian Haß
         */
        inline void addJob(Process job);

        /**
         * @brief execute
         *
         * Executes all jobs concurrently and returns the results
         * of those jobs.
         *
         * @return Returns the result of the jobs.
         *
         * @attention The order of result may differ from the order of the
         *            assigned jobs. This depends on the execution speed of
         *            each single job as well as the cpu time availability
         *            per thread. It may be required to add an order value
         *            to the job and the result to be able to sort the results
         *            after the execution.
         *
         * @author Christian Haß
         */
        inline void execute();

      protected:
        /// Reference to the used thread pool.
        Threadpool& pool;
        /// List of jobs that needs to be performed.
        std::vector<Process> jobs;
    };

    inline ThreadManager<void,void>::ThreadManager(Threadpool& pool)
      : pool(pool)
    {}

    inline ThreadManager<void,void>::ThreadManager(Threadpool& pool, std::vector<Process>&& jobs)
      : pool(pool),
        jobs(std::move(jobs))
    {}

    inline void ThreadManager<void,void>::reserve(std::size_t amount) {
      jobs.reserve(amount);
    }

    inline void ThreadManager<void,void>::addJob(Process job) {
      jobs.emplace_back(std::move(job));
    }

    inline void ThreadManager<void, void>::execute() {
      return batch(pool, std::move(jobs));
    }
  }
}
