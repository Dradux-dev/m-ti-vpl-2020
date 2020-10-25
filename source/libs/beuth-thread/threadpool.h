#pragma once

#include <algorithm>
#include <future>

#include "beuth-thread_global.h"

#include "counting-semaphore.hpp"
#include "thread.h"

namespace beuth {
  namespace thread {
    class BEUTHTHREAD_EXPORT Threadpool : public Thread::Parent {
      public:
        using Process = std::function<void()>;

        explicit inline Threadpool(std::size_t count);
        virtual ~Threadpool();

        std::future<void> process(Process p);
        void process(std::vector<Process>&& vp);

        inline void stop();
        inline void forceStop();

        [[nodiscard]] inline std::size_t getSize() const noexcept;

      protected:
        inline virtual void threadIsReady(Thread* thread) override;
        inline void waitForThread(std::size_t amount = 1);
        [[nodiscard]] inline Thread* getAvailableThread() noexcept;

      protected:
        CountingSemaphore threadsAvailable;

        std::vector<Thread*> threads;

        std::mutex availableMutex;
        std::queue<Thread*> available;
    };

    inline Threadpool::Threadpool(std::size_t count)
      : threadsAvailable(0)
    {
      threads.reserve(count);

      for (std::size_t n = 0; n < count; ++n) {
        Thread* thread = new Thread(this);
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

    inline void Threadpool::threadIsReady(Thread* thread) {
      {
        std::lock_guard<std::mutex> guard(availableMutex);
        available.push(thread);
      }

      threadsAvailable.give();
    }

    inline void Threadpool::waitForThread(std::size_t amount) {
      threadsAvailable.take(amount);
    }

    [[nodiscard]] inline Thread* Threadpool::getAvailableThread() noexcept {
      waitForThread();

      std::lock_guard<std::mutex> guard(availableMutex);
      Thread* thread = available.front();
      available.pop();

      return thread;
    }
  }
}
