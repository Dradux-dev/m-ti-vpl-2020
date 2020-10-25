#pragma once

#include "counting-semaphore.hpp"
#include "threadpool.h"

namespace beuth {
  namespace thread {
    /**
     * @brief Batch of multi threading tasks
     *
     * Will assign tasks to a thread pool as long as threads are available. If no thread is currently available the batch will block
     * the calling thread until a thread is available for the new task.
     *
     * @author Tarek Schwarzinger
     */
    class Batch {
      public:
        /// Process function pointer signature
        using Process = Threadpool::Process;

        /**
         * @brief Constructor
         *
         * Constructs a batch.
         *
         * @param pool The thread pool that shall be used for assigning tasks.
         * @param size The amount of tasks, that will get assigned. If already known at batch creation.
         *
         * @attention Assigning the correct size in the constructor will save runtime, due to no
         *            resize required.
         *
         * @author Tarek Schwarzinger
         */
        inline explicit Batch(Threadpool& pool, std::size_t size = 100)
          : pool(pool)
        {
          states.reserve(size);
        }

        /**
         * @brief Start process
         *
         * Takes a process and assigns it to the thread pool.
         *
         * @param p The process that shall be executed in it's own thread.
         *
         * @author Tarek Schwarzinger
         */
        inline void process(Process p) {
            if (!p) {
              return;
            }

            states.emplace_back(pool.process(p));
        }

        /**
         * @brief wait
         *
         * Waits for all processes of this batch to finish.
         *
         * @author Tarek Schwarzinger
         */
        inline void wait() {
          for (std::future<void>& f : states) {
            f.wait();
          }
        }

      protected:
        /// Reference to the used thread pool
        Threadpool& pool;

        /// State of used threads to process tasks
        std::vector<std::future<void>> states;
    };

    /**
     * @brief batch
     *
     * Creates a batch from a job list and takes care of the result management.
     *
     * @tparam TJob Type of the job structure.
     * @tparam TResult Type of the result structure.
     *
     * @param pool The thread pool that shall get the jobs assigned.
     * @param jobs List of the jobs that shall be performed.
     * @param processor Function that can perform a job to create the corresponding result out of it.
     *
     * @return Returns the list of results, that have been created for the jobs.
     *
     * @author Tarek Schwarzinger
     */
    template <typename TJob, typename TResult>
    std::vector<TResult> batch(Threadpool& pool, std::vector<TJob>&& jobs, std::function<TResult(TJob&&)> processor) {
      std::mutex resultLock;
      std::vector<TResult> result;

      Batch batch(pool);

      for (std::size_t n = 0; n < jobs.size(); ++n) {
        batch.process([&resultLock, &result, &processor, &jobs, n] {
          TResult r = processor(std::move(jobs[n]));

          resultLock.lock();
          result.emplace_back(std::move(r));
          resultLock.unlock();
        });
      }

      batch.wait();
      return std::move(result);
    }

    /**
     * @brief batch
     *
     * Takes a list ob jobs, that does not generate a result and performs those concurrently.
     *
     * @tparam TJob Type of the job structure.
     *
     * @param pool The thread pool that shall be used to perform the jobs.
     * @param jobs The list of jobs, that shall be performed.
     * @param processor Function that is able to perform a job.
     *
     * @author Tarek Schwarzinger
     */
    template <typename TJob>
    void batch(Threadpool& pool, std::vector<TJob>&& jobs, std::function<void(TJob&&)> processor) {
      Batch batch(pool);

      for(TJob& job : jobs) {
        batch.process([&] {
            processor(std::move(job));
        });
      }

      batch.wait();
    }

    /**
     * @brief batch
     *
     * Takes a list of functions and performs those concurrently.
     *
     * @param pool The thread pool, that shall be used to perform the jobs.
     * @param jobs List of functions, that has to be performed concurrently.
     *
     * @author Tarek Schwarzinger
     */
    void batch(Threadpool& pool, std::vector<std::function<void()>>&& jobs) {
      Batch batch(pool);

      for(std::function<void()> job : jobs) {
        batch.process(job);
      }

      batch.wait();
    }
  }
}
