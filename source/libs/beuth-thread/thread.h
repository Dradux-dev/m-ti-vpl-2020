#pragma once

#include <atomic>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "beuth-thread_global.h"
#include "counting-semaphore.hpp"

//#define BEUTH_THREAD_ENABLE_DEBUG_HELPER 1
//#define BEUTH_THREAD_ENABLE_DEBUG_HISTORY 1

#ifdef BEUTH_THREAD_ENABLE_DEBUG_HISTORY
# define BEUTH_THREAD_HISTORY(message) history(message)
#else
# define BEUTH_THREAD_HISTORY(message)
#endif

namespace beuth {
  namespace thread {
    class BEUTHTHREAD_EXPORT Thread : public std::thread
    {
      public:
        struct Parent {
          friend class Thread;

          protected:
            virtual void threadIsReady(Thread*) = 0;
        };

        using Process = std::function<void()>;
        using Task = std::packaged_task<void()>;

        inline explicit Thread(Parent* parent);
        virtual ~Thread();

        void run();

        inline std::future<void> wakeUp(Process p);
        inline void waitForProcess();
        inline void waitForStartup();
        inline void sleep();
        inline void sleepWhenDone();
        inline void forceStop();
        inline void stop();

        [[nodiscard]] inline bool isBusy() const noexcept;
        [[nodiscard]] inline bool isStopped() const noexcept;
        [[nodiscard]] inline bool isAvailable() const noexcept;

      protected:
        inline void signalReady();

        [[nodiscard]] inline bool isStopping() const noexcept;
        [[nodiscard]] inline bool isRunning() const noexcept;
        [[nodiscard]] inline bool hasTaskAssigned() const noexcept;
        [[nodiscard]] inline bool isPerformingATask() const noexcept;

      private:
        Parent* parent = nullptr;
        Task task = Task();

        // true = shall stop; false = can stay alive
        std::atomic_bool shallStop;

        // 0 = is running; 1 = is not running
        CountingSemaphore running;

        // 0 = no task to perform; 1 = task to perform (including stopping)
        CountingSemaphore taskCount;

        // 0 = performing task; 1 = is not performing a task
        CountingSemaphore ready;

#ifdef BEUTH_THREAD_ENABLE_DEBUG_HELPER
        struct {
            bool shallStop;
            bool running;
            bool taskAssigned;
            bool ready;
            bool taskPerformed;
            struct {
                bool busy;
                bool stopped;
                bool available;
                bool stopping;
                bool running;
                bool taskAssigned;
                bool performingTask;
            } functions;
            struct {
              volatile int assigned{0};
              volatile int started{0};
              volatile int performed{0};
            } tasks;
        } debug_helper;
#endif

#ifdef BEUTH_THREAD_ENABLE_DEBUG_HISTORY
        std::mutex dbg_history_mutex;
        std::vector<std::string> dbg_history;

        inline void history(const char* s) {
          std::lock_guard<std::mutex> guard(dbg_history_mutex);
          if (dbg_history.size() >= 100) {
            dbg_history.erase(dbg_history.begin());
          }

          dbg_history.emplace_back(s);
        }
#endif
    };

    inline Thread::Thread(Parent* parent)
      : std::thread(&Thread::run,this),
        parent(parent),
        shallStop(false),
        running(1),
        taskCount(0),
        ready(1)
    {
#ifdef BEUTH_THREAD_ENABLE_DEBUG_HISTORY
      std::lock_guard<std::mutex> guard(dbg_history_mutex);
      dbg_history.reserve(100);
#endif
    }

    [[nodiscard]] inline bool Thread::isBusy() const noexcept
    {
        return isPerformingATask() || hasTaskAssigned();
    }

    [[nodiscard]] inline bool Thread::isStopped() const noexcept
    {
        return !isRunning() && shallStop;
    }

    [[nodiscard]] inline bool Thread::isAvailable() const noexcept {
      return !isBusy() && !isStopped();
    }

    inline void Thread::forceStop()
    {
        shallStop = true;
        taskCount.give();
    }

    inline void Thread::stop()
    {
        waitForProcess();
        shallStop = true;
        taskCount.give();
    }

    inline std::future<void> Thread::wakeUp(Process p)
    {
        BEUTH_THREAD_HISTORY("wakeUp");

        if (!p) {
          return std::future<void>();
        }

        if (shallStop) {
          return std::future<void>();
        }

#ifdef BEUTH_THREAD_ENABLE_DEBUG_HELPER
        debug_helper = {
          .shallStop = shallStop,
          .running = isRunning(),
          .taskAssigned = (static_cast<std::ptrdiff_t>(taskCount) > 0),
          .ready = (static_cast<std::ptrdiff_t>(taskCount) > 0),
          .taskPerformed = false,
          .functions = {
            .busy = isBusy(),
            .stopped = isStopped(),
            .available = isAvailable(),
            .stopping = isStopping(),
            .running = isRunning(),
            .taskAssigned = hasTaskAssigned(),
            .performingTask = isPerformingATask()
          },
          .tasks = debug_helper.tasks
        };

        ++debug_helper.tasks.assigned;
#endif

        this->task = std::packaged_task<void()>(p);
        taskCount.give();

        return task.get_future();
    }

    inline void Thread::waitForProcess()
    {
      ready.wait([](std::ptrdiff_t count) -> bool {
        return count > 0;
      });
    }

    [[nodiscard]] inline bool Thread::isStopping() const noexcept {
      return shallStop;
    }

    [[nodiscard]] inline bool Thread::isRunning() const noexcept {
      return static_cast<std::ptrdiff_t>(running) == 0;
    }

    [[nodiscard]] inline bool Thread::hasTaskAssigned() const noexcept {
      return static_cast<std::ptrdiff_t>(taskCount) > 0;
    }

    [[nodiscard]] inline bool Thread::isPerformingATask() const noexcept {
      return static_cast<std::ptrdiff_t>(ready) == 0;
    }
  }
}


