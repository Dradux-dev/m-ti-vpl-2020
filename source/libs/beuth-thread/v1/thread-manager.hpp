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
    namespace v1 {
      template <typename TJob, typename TResult>
      class ThreadManager {
        public:
          using Job = TJob;
          using Result = TResult;
          using Process = std::function<Result(Job&&)>;

          explicit inline ThreadManager(Threadpool& pool, Process process);
          explicit inline ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process proccess);

          inline void reserve(std::size_t amount);
          inline void addJob(Job&& job);

          inline std::vector<Result> execute();

        protected:
          Threadpool& pool;
          std::vector<Job> jobs;
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



      template <typename TJob>
      class ThreadManager<TJob, void> {
        public:
          using Job = TJob;
          using Process = std::function<void(Job&&)>;

          explicit inline ThreadManager(Threadpool& pool, Process process);
          explicit inline ThreadManager(Threadpool& pool, std::vector<Job>&& jobs, Process proccess);

          inline void reserve(std::size_t amount);
          inline void addJob(Job&& job);

          inline void execute();

        protected:
          Threadpool& pool;
          std::vector<Job> jobs;
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

      template <>
      class ThreadManager<void, void> {
        public:
          using Process = Threadpool::Process;

          explicit inline ThreadManager(Threadpool& pool);
          explicit inline ThreadManager(Threadpool& pool, std::vector<Process>&& jobs);

          inline void reserve(std::size_t amount);
          inline void addJob(Process job);

          inline void execute();

        protected:
          Threadpool& pool;
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
}
