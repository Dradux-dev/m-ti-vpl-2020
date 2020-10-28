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
    /**
     * @brief Task based thread
     *
     * Thread class that accepts tasks to perform. If no task is assigned to
     * this thread, it sleeps until it a new task is assigned.
     *
     * @attention This thread has no queue or backlog. It is only able to
     *            accept one task at a given time.
     *
     * @author Tarek Schwarzinger
     */
    class BEUTHTHREAD_EXPORT Thread
    {
      public:
        /**
         * @brief Thread parent
         *
         * Interface that is used by the thread to notify, when ready for
         * getting a new task assigned.
         *
         * @author Tarek Schwarzinger
         */
        struct Parent {
          /// Allow thread access protected signal
          friend class Thread;

          protected:
            /**
             * @brief Thread is ready
             *
             * Signal for the thread to notify it's parent, that it is ready
             * to get a new task assigned.
             *
             * @param thread The thread that is ready for a new task.
             *
             * @author Tarek Schwarzinger
             */
            virtual void threadIsReady(Thread* thread) = 0;
        };

        /// Type definition of a process that shall be performed as task
        using Process = std::function<void()>;
        /// Type definition of a task
        using Task = std::packaged_task<void()>;

        /**
         * @brief Constructor
         *
         * Initializes a thread with a parent.
         *
         * @param parent Parent of the thread, that will assign tasks to it.
         *
         * @author Tarek Schwarzinger
         */
        inline explicit Thread(Parent* parent);

        /**
         * @brief Destructor
         *
         * Destructs a thread and joins it, if not already done.
         *
         * @author Tarek Schwarzinger
         */
        virtual ~Thread();

        /**
         * @brief run
         *
         * The function that will be performed within the new thread. This function
         * will be able to perform assigned tasks.
         *
         * @author Tarek Schwarzinger
         */
        void run();

        /**
         * @brief Wake up
         *
         * Wakes up the thread to perform a new task. The task is defined by a process
         * that needs to be executed
         *
         * @param p The process that needs to be assigned as new task.
         *
         * @return Returns a future, that will be notified when the task has been completed.
         *
         * @retval Empty If the process was declined by the wakeUp() function an empty future
         *               is returned. This will be the case if either the process is a null
         *               pointer or the thread shall stop it's execution for shutdown.
         * @retval Valid Returns a valid future corresponding to the task promise that has been made.
         *
         * @author Tarek Schwarzinger
         */
        inline std::future<void> wakeUp(Process p);

        /**
         * @brief Wait for process
         *
         * This function waits, until the current task has been performed.
         *
         * @attention This is blocking call.
         *
         * @author Tarek Schwarzinger
         */
        inline void waitForProcess();

        /**
         * @brief start
         *
         * Starts the thread. The thread will notify it's parent as soon as it is ready to get a
         * task assigned.
         *
         * @see signalReady()
         *
         * @author Tarek Schwarzinger
         */
        inline void start();

        /**
         * @brief Force stop
         *
         * Immediately assigns the task to shutdown this thread.
         *
         * @attention This will not break the execution of an already assigned task.
         *
         * @author Tarek Schwarzinger
         */
        inline void forceStop();

        /**
         * @brief stop
         *
         * Assigns the task to shutdown this thread.
         *
         * @attention This is a blocking call, which will wait until the thread has
         * finished it's currently assigned task.
         *
         * @author Tarek Schwarzinger
         */
        inline void stop();

        /**
         * @brief is busy?
         *
         * Checks, if this thread is busy at the moment.
         *
         * @return Returns the current busy state of the thread.
         *
         * @retval true The thread is currently busy.
         * @retval false The thread has no current task.
         *
         * @test "Thread/busy" proves that the is busy function
         *       returns true, if the thread has a task assigned.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isBusy() const noexcept;

        /**
         * @brief is stopped?
         *
         * Checks, if this thread is stopped.
         *
         * @return Returns the current stop state of the thread.
         *
         * @retval true The thread is stopped.
         * @retval false The thread is not stopped.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isStopped() const noexcept;

        /**
         * is available?
         *
         * Checks, if the thread is available for a new task.
         *
         * @return Returns the current availability of the thread.
         * @retval true The thread is ready for a new task.
         * @retval false The thread is not ready for a new task.
         *
         * @attention Assigning a new job requires some time to change
         *            the availability state. Fast polling on this state
         *            can result in a race condition. It is highly recommended
         *            to wait for the signal to it's parent, that the thread is
         *            ready for a new job.
         *
         * @test "Thread/available" proves that the thread is not available while
         *       performing a task and that the thread is available, after the task
         *       has been finished.
         *
         * @see signalReady()
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isAvailable() const noexcept;

      protected:
        /**
         * @brief signal ready
         *
         * Signals it's parent, that this thread is ready to get a new task assigned.
         *
         * @see Parent::threadIsReady()
         *
         * @test "Thread/ready" proves that signalling ready of the thread works as intended.
         *
         * @author Tarek Schwarzinger
         */
        inline void signalReady();

        /**
         * @brief is stopping?
         *
         * Checks, if the thread is currently stopping.
         *
         * @attention This will be the case as soon as the stop has been assigned as new
         *            task. If assigned with forceStop(), this will be the case no matter
         *            how long the currently assigned task will take before the stop can be
         *            initiated.
         *
         * @return Returns, if the thread is stopping.
         * @retval true The stop task is assigned.
         * @retval false The stop task is not already assigned.
         *
         * @see stop()
         * @see forceStop()
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isStopping() const noexcept;

        /**
         * @brief is running?
         *
         * Checks, if the thread is already running.
         *
         * @return Returns if the thread is started and running.
         * @retval true The thread is running.
         * @retval false The thread is not running.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isRunning() const noexcept;

        /**
         * @brief has task assigned?
         *
         * Checks, if the current thread has already a task assigned.
         *
         * @return Returns if the thread has a task assigned.
         * @retval true The thread has a task assigned.
         * @retval false The thread has no task assigned.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool hasTaskAssigned() const noexcept;

        /**
         * @brief is performing task?
         *
         * Checks, if the thread is currently performing a task.
         *
         * @return Returns if the thread is currently performing an assigned task.
         * @retval true Is performing a task.
         * @retval false Is not performing a task.
         *
         * @author Tarek Schwarzinger
         */
        [[nodiscard]] inline bool isPerformingATask() const noexcept;

      private:
        /// Parent of the thread
        Parent* parent = nullptr;

        // Thread itself
        std::thread thread;

        /// Currently assigned task
        Task task = Task();

        //// true = shall stop; false = can stay alive
        std::atomic_bool shallStop;

        /// 0 = is running; 1 = is not running
        CountingSemaphore running;

        /// 0 = no task to perform; 1 = task to perform (including stopping)
        CountingSemaphore taskCount;

        /// 0 = performing task; 1 = is not performing a task
        CountingSemaphore ready;

#ifdef BEUTH_THREAD_ENABLE_DEBUG_HELPER
        /**
         * @brief debug helper
         *
         * Stores information, when the thread gets a new task assigned. These are meant
         * to help debugging the thread in case of an error, without changing the timing
         * to much like it would be with a breakpoint or stdout dumps.
         *
         * @attention This feature is disabled by default and needs to be enabled
         * with -DBEUTH_THREAD_ENABLE_DEBUG_HELPER compiler flag.
         */
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
        /// Mutex to guard db_history (requires -DBEUTH_THREAD_ENABLE_DEBUG_HISTORY)
        std::mutex dbg_history_mutex;
        /// List of events that occured within the thread (requires -DBEUTH_THREAD_ENABLE_DEBUG_HISTORY)
        std::vector<std::string> dbg_history;

        /**
         * @brief history
         *
         * Stores an informational string inside the history log of the thread. This is meant to help debugging
         * the thread. This function requires the -DBEUTH_THREAD_ENABLE_DEBUG_HISTORY compiler flag to be available.
         *
         * @param s The message that shall be stored in the history log.
         *
         * @note The history is limited to 100 messages, which should be enough to check the last couple of task
         *       assignments as well as executions.
         *
         * @attention Do not use this function directly. Use BEUTH_THREAD_HISTORY instead, which takes automatically
         *            care of the compiler flag and automatically removes all history message, if the compiler flag
         *            is missing.
         *
         * @author Tarek Schwarzinger
         */
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
      : parent(parent),
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

    inline void Thread::start() {
      thread = std::thread(&Thread::run,this);
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
      // Wait until thread is started
      running.wait([](std::ptrdiff_t count) -> bool {
        return count == 0;
      });

      // Wait until thread is ready for new task
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


