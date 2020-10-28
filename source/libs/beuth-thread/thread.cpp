#include "thread.h"

#include <cassert>

namespace beuth {
  namespace thread {
    Thread::~Thread()
    {
        if (thread.joinable()) {
            thread.join();
        }
    }

    void Thread::run()
    {
        running.take();

        while(!shallStop)
        {
            BEUTH_THREAD_HISTORY("signalReady");
            signalReady();

            BEUTH_THREAD_HISTORY("waitForTask");
            taskCount.take();

            BEUTH_THREAD_HISTORY("gotTask");
            ready.take();

            BEUTH_THREAD_HISTORY("runTask");
            if(shallStop)
            {
                BEUTH_THREAD_HISTORY("shallStop");
                continue;
            }

            try {
              BEUTH_THREAD_HISTORY("performTask");
#ifdef BEUTH_THREAD_ENABLE_DEBUG_HELPER
              ++debug_helper.tasks.started;
#endif
              task();
#ifdef BEUTH_THREAD_ENABLE_DEBUG_HELPER
              ++debug_helper.tasks.performed;
              debug_helper.taskPerformed = true;
#endif
            }
            catch(std::future_error& e) {
              std::cout << e.what() << std::endl;
            }

            ready.give();
        }

        running.give();
    }

    inline void Thread::signalReady() {
      if (parent) {
        parent->threadIsReady(this);
      }
    }
  }
}
