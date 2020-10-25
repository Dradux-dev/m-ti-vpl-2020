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

        inline void process(Process p) {
            if (!p) {
              return;
            }

            states.emplace_back(pool.process(p));
        }

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

    template <typename TJob, typename TResult>
    std::vector<TResult> batch(Threadpool& pool, std::vector<TJob>&& jobs, std::function<TResult(TJob&&)> processor) {
      std::mutex resultLock;
      std::vector<TResult> result;

      Batch batch(pool);

      for (std::size_t n = 0; n < jobs.size(); ++n) {
        batch.process([&resultLock, &result, &processor, &jobs, n] {
//          std::cout << "Task started" << std::flush << std::endl;
//          std::cout << "S" << std::flush;
          TResult r = processor(std::move(jobs[n]));

          resultLock.lock();
          result.emplace_back(std::move(r));
          resultLock.unlock();
//          std::cout << "Task finished" << std::flush << std::endl;
//          std::cout << "F" << std::flush;
        });
      }

      batch.wait();
      return std::move(result);
    }

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

    void batch(Threadpool& pool, std::vector<std::function<void()>>&& jobs) {
      Batch batch(pool);

      for(std::function<void()> job : jobs) {
        batch.process(job);
      }

      batch.wait();
    }
  }
}
