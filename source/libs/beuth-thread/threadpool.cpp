#include "threadpool.h"

#include "batch.hpp"

namespace beuth {
  namespace thread {
    Threadpool::~Threadpool() {
      forceStop();

      for (Thread* thread : threads) {
        delete thread;
      }
    }

    Thread* Threadpool::process(Process p) {
      waitForThread();

      for(Thread* t : threads) {
        if (t->isAvailable()) {
          t->wakeUp(p);
          return t;
        }
      }

      return nullptr;
    }

    void Threadpool::process(std::vector<Process>&& vp) {
      Batch batch(*this);

      for (Process p : vp) {
        batch.process(p);
      }

      batch.wait();
    }
  }
}
