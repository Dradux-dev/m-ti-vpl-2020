#pragma once

#include "counting-semaphore.hpp"
#include "threadpool.h"

namespace beuth {
  namespace thread {
    class Batch {
      public:
        using Process = Threadpool::Process;

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
              std::lock_guard<std::mutex> guard(m);
              states.emplace_back(thread);
            }
          } while (!thread);
        }

        inline void wait() {
          m.lock();
          for (Thread* thread : states) {
            if (thread) {
              m.unlock();
              thread->waitForProcess();
              m.lock();
            }
          }
          m.unlock();
        }
      protected:
        Threadpool& pool;
        std::mutex m;
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
