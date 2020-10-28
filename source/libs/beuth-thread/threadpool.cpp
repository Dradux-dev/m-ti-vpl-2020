#include "threadpool.h"

#include <cassert>
#include <iomanip>
#include <iostream>

#include "batch.hpp"

namespace beuth {
  namespace thread {
    Threadpool::~Threadpool() {
      stop();

      for (Thread* thread : threads) {
        delete thread;
      }
    }

    std::future<void> Threadpool::process(Process p) {
      Thread* thread = getAvailableThread();
      assert(thread);
      return thread->wakeUp(p);
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
