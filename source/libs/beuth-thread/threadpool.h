#pragma once

#include <algorithm>
#include <future>

#include "beuth-thread_global.h"

#include "counting-semaphore.hpp"
#include "thread.h"

namespace beuth {
  namespace thread {
    /**
     * @brief Thread pool
     *
     * Pool of threads, that can be used to concurrently perform any given tasks.
     *
     * @author Tarek Schwarzinger
     */
    class BEUTHTHREAD_EXPORT Threadpool : public Thread::Parent {
      public:
        /// Function signature of a task
        using Process = std::function<void()>;

        /**
         * @brief Constructor
         *
         * Initializes the thread pool by creating and starting count threads. All threads
         * go immediately into sleep state waiting to get a task assigned to it.
         *
         * @param count The number of threads that shall be started.
         *
         * @author Tarek Schwarzinger
         */
        explicit inline Threadpool(std::size_t count);

        /**
         * @brief Destructor
         *
         * Stops and destorys all threads.
         *
         * @author Tarek Schwarzinger
         */
        virtual ~Threadpool();

        /**
         * @brief process
         *
         * Assigns a process to a thread to be executed as task.
         *
         * @param p The process that shall be performed.
         *
         * @return Returns the corresponding future to that promise.
         *
         * @attention This is a blocking call. It will wait for a thread to
         *            be ready for a new task, if no thread was available when
         *            this function was called.
         *
         * @author Tarek Schwarzinger
         */
        std::future<void> process(Process p);

        /**
         * @brief process
         *
         * Assigns every single process of the list to a thread to be performed.
         *
         * @param vp List of processes that need to be performed.
         *
         * @attention This is blocking call, which will wait until all process have
         *            been assigned and completely performed.
         *
         * @author Tarek Schwarzinger
         */
        void process(std::vector<Process>&& vp);

        /**
         * @brief stop
         *
         * Stops all threads.
         *
         * @attention This is blocking call, which will wait for every single thread
         *            to be ready for the stop task.
         *
         * @author Tarek Schwarzinger
         */
        inline void stop();

        /**
         * @brief force stop
         *
         * Stops all threads.
         *
         * @attention It may take a while until all threads are stopped. This function only assigns the
         *            task to stop immediately, but it will not cancel the currently performed task of
         *            the thread.
         *
         * @author Tarek Schwarzinger
         */
        inline void forceStop();

        /**
         * @brief thread amount
         *
         * @return Returns the amount of threads within this thread pool.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline std::size_t getSize() const noexcept;

      protected:
        /**
         * @brief thread is ready
         *
         * This function is automatically called, whenever a thread is ready for a new task.
         *
         * @param thread The thread that is ready for a new task.
         *
         * @attention Do not call this function manually. This should be only called
         *           from within a thread to signal, that it is ready for a new task.
         *
         * @see Thread::Parent
         *
         * @author Tarek Schwarzinger
         */
        inline virtual void threadIsReady(Thread* thread) override;

        /**
         * @brief wait for thread
         *
         * Waits for the given amount of threads to be available for new tasks.
         *
         * @param amount Amount of threads that is required. By default only one thread is required.
         *
         * @attention This is a blocking call, which will reserve the amount of threads for the caller. If
         *            the calling thread is not assigning tasks to the threads these won't be available
         *            for anyone again. It is mandatory to assign tasks to the reserved threads.
         *
         * @author Tarek Schwarzinger
         */
        inline void waitForThread(std::size_t amount = 1);

        /**
         * @brief get available thread
         *
         * @return Returns the pointer to an available thread, that is waiting for a new task.
         *
         * @note This function is not capable of returning a null pointer theoretically.
         *
         * @attention This is a blocking call, which reserves the thread for the calling
         *            thread. It is mandatory to assign a task to the reserved thread.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline Thread* getAvailableThread() noexcept;

      protected:
        /// Counting semaphore of threads, that are waiting for new tasks
        CountingSemaphore threadsAvailable;

        /// List of threads owned and managed by the thread pool
        std::vector<Thread*> threads;

        /// Guard for the queue of available threads
        std::mutex availableMutex;
        /// Queue of threads that are waiting for a new task
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
