#include "thread.h"

#include "threadpool.h"

namespace beuth {
  namespace thread {
    namespace v1 {
      Thread::~Thread()
      {
          if (joinable()) {
              join();
          }
      }

      void Thread::run()
      {
          started = true;
          startedCondition.notify_all();

          while(!shallStop)
          {
              // parent->I_am_ready();
              std::unique_lock<std::mutex> lock(sleepMutex);
              sleepCondition.wait(lock, [this] {
                 // [Goal]
                 // Sleep Stop | r
                 //     0    0   1
                 //     0    1   1
                 //     1    0   0
                 //     1    1   1
                std::lock_guard<std::mutex> shallSleeGuard(shallSleepMutex);
                 return this->shallStop || !this->shallSleep;
                 // [Current State]
                 // Sleep Stop | Stop !Sleep | f
                 //     0    0 |    0      1 | 1
                 //     0    1 |    1      1 | 1
                 //     1    0 |    0      0 | 0
                 //     1    1 |    1      0 | 1
              });

              if (shallStop)
              {
                  continue;
              }

              if(process)
              {
                  busyMutex.try_lock();

                  processingStarted();
                  process();
                  processingFinished();

                  process = nullptr;
                  busyMutex.unlock();
                  busyCondition.notify_one();
              }

              lock.unlock();
          }

          busyMutex.unlock();
          busyCondition.notify_one();
      }

      inline void Thread::processingStarted() {
        isProcessing = true;
        parent->processStarted(this);
      }

      inline void Thread::processingFinished() {
        isProcessing = false;
        parent->processFinished(this);
      }
    }
  }
}
