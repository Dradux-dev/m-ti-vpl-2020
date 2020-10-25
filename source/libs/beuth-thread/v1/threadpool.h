#pragma once

#include <algorithm>

#include "beuth-thread_global.h"

#include "counting-semaphore.hpp"
#include "thread.h"

namespace beuth {
  namespace thread {
    namespace v1 {
      class BEUTHTHREAD_EXPORT Threadpool {
        public:
          friend class Thread;

          using Process = Thread::Process;

          explicit inline Threadpool(std::size_t count);
          virtual ~Threadpool();

          Thread* process(Process p);
          void process(std::vector<Process>&& vp);

          inline void stop();
          inline void forceStop();

          [[nodiscard]] inline std::size_t getSize() const noexcept;

        protected:
          inline void processStarted(Thread* thread);
          inline void processFinished(Thread* thread);
          inline void waitForThread(std::size_t amount = 1);

        protected:
          std::vector<Thread*> threads;
          CountingSemaphore threadsAvailable;
      };

      inline Threadpool::Threadpool(std::size_t count)
        : threadsAvailable(count)
      {
        threads.reserve(count);
        for (std::size_t n = 0; n < count; ++n) {
          Thread* thread = new Thread(this);
          thread->waitForStartup();
          threads.emplace_back(thread);
        }
      }

      inline void Threadpool::stop() {
        for (Thread* t : threads) {
          t->stop();
        }
      }

      inline void Threadpool::forceStop() {
        for (Thread* t : threads) {
          t->forceStop();
        }
      }

      [[nodiscard]] inline std::size_t Threadpool::getSize() const noexcept {
        return threads.size();
      }

      inline void Threadpool::processStarted(Thread*) {
      }

      inline void Threadpool::processFinished(Thread* thread) {
        thread->sleep();
        threadsAvailable.give();
      }

      inline void Threadpool::waitForThread(std::size_t amount) {
        threadsAvailable.take(amount);
      }
    }
  }
}
