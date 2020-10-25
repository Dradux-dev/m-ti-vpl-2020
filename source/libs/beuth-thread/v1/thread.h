#pragma once

#include <atomic>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "beuth-thread_global.h"

namespace beuth {
  namespace thread {
    namespace v1 {
      class BEUTHTHREAD_EXPORT Thread : public std::thread
      {
        public:
          using lock_guard = std::lock_guard<std::mutex>;
          using unique_lock = std::unique_lock<std::mutex>;
          using Process = std::function<void(void)>;

          inline explicit Thread(class Threadpool* parent);
          virtual ~Thread();

          void run();

          inline void wakeUp(Process process);
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
          inline void processingStarted();
          inline void processingFinished();

        private:
          class Threadpool* parent = nullptr;
          Process process = nullptr;
          volatile bool isProcessing = false;
          mutable std::mutex isProcessingMutex;
          std::mutex sleepMutex;
          std::condition_variable sleepCondition;
          volatile bool shallSleep = true;
          mutable std::mutex shallSleepMutex;
          std::mutex busyMutex;
          std::condition_variable busyCondition;
          volatile bool shallStop = false;
          std::atomic_bool started;
          std::mutex startedMutex;
          std::condition_variable startedCondition;
      };

      inline Thread::Thread(class Threadpool* parent)
        : std::thread(&Thread::run,this),
          parent(parent),
          started(false)
      {
      }

      [[nodiscard]] inline bool Thread::isBusy() const noexcept
      {
          std::lock_guard<std::mutex> isProcessingGuard(isProcessingMutex);
          std::lock_guard<std::mutex> shallSleeGuard(shallSleepMutex);
          return isProcessing || !shallSleep;
      }

      [[nodiscard]] inline bool Thread::isStopped() const noexcept
      {
          return shallStop;
      }

      [[nodiscard]] inline bool Thread::isAvailable() const noexcept {
        return !isBusy() && !isStopped();
      }

      inline void Thread::forceStop()
      {
          shallStop = true;
          sleepCondition.notify_one();
      }

      inline void Thread::stop()
      {
          waitForProcess();
          shallStop = true;
          sleepCondition.notify_one();
      }

      inline void Thread::sleepWhenDone()
      {
          waitForProcess();

          std::lock_guard<std::mutex> shallSleeGuard(shallSleepMutex);
          shallSleep = true;
      }

      inline void Thread::sleep()
      {
          std::lock_guard<std::mutex> shallSleeGuard(shallSleepMutex);
          shallSleep = true;
          /// @todo test if really required
          //sleepCondition.notify_one();
      }

      inline void Thread::wakeUp(Process process)
      {
          if (shallStop) {
            return;
          }

          this->process = process;

          std::lock_guard<std::mutex> shallSleeGuard(shallSleepMutex);
          shallSleep = false;
          sleepCondition.notify_one();
      }

      inline void Thread::waitForProcess()
      {
          unique_lock lk(busyMutex);
          busyCondition.wait(lk, [this] { return !this->isBusy(); });
      }

      inline void Thread::waitForStartup() {
        unique_lock lk(startedMutex);
        startedCondition.wait(lk, [this] { return static_cast<bool>(started); });
      }
    }
  }
}


