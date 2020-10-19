#pragma once

#include <algorithm>

#include "beuth-thread_global.h"

#include "counting-semaphore.hpp"
#include "thread.h"

namespace beuth {
  namespace thread {
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
        inline void waitForThread();

      protected:
        std::vector<Thread*> threads;
        CountingSemaphore threadsAvailable;
    };

    inline Threadpool::Threadpool(std::size_t count)
      : threadsAvailable(count)
    {
      threads.reserve(count);
      for (std::size_t n = 0; n < count; ++n) {
        threads.emplace_back(new Thread(this));
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
      threadsAvailable.reduce();
    }

    inline void Threadpool::processFinished(Thread* thread) {
      thread->sleep();
      threadsAvailable.give();
    }

    inline void Threadpool::waitForThread() {
      threadsAvailable.wait([](std::ptrdiff_t count) -> bool {
        return count >= 1;
      });
    }
  }
}
