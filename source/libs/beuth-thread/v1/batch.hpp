#pragma once

#include "counting-semaphore.hpp"
#include "threadpool.h"

namespace beuth {
  namespace thread {
    namespace v1 {
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

          inline void done(std::size_t n) {
            std::lock_guard<std::mutex> guard(m);
            states[n] = nullptr;
          }

          inline void process(Process p) {
  //          std::cout << "Batch::process() called" << std::endl;

            Thread* thread = nullptr;
            do {
              std::size_t n = states.size();

              thread = pool.process([this, p, n] {
                if (p) {
                  p();
                }

                this->done(n);
              });

              if (thread) {
  //              std::cout << "Batch::process() Thread found" << std::endl;
                std::lock_guard<std::mutex> guard(m);
                states.emplace_back(thread);
              }
              else {
  //              std::cout << "Batch::process() No Thread available" << std::endl;
              }
            } while (!thread);
          }

          inline void wait() {
            std::cout << "Batch::wait() called" << std::endl;
            m.lock();
  //          std::cout << "Batch::wait() locked" << std::endl;
            for (Thread* thread : states) {
              if (thread) {
  //              std::cout << "Batch::wait() processing thread ..." << std::endl;
                m.unlock();
  //              std::cout << "Batch::wait() waiting for thread ..." << std::endl;
                thread->waitForProcess();
  //              std::cout << "Batch::wait() locking thread ..." << std::endl;
                m.lock();
  //              std::cout << "Batch::wait() thread locked ..." << std::endl;
              }
              else {
  //              std::cout << "Batch::wait() omitting thread ..." << std::endl;
              }
            }
            m.unlock();
          }
        protected:
          /// Reference to the used thread pool
          Threadpool& pool;

          /// Mutex to protect the thread state vector
          std::mutex m;

          /// State of used threads to process tasks
          std::vector<Thread*> states;
      };

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
}
