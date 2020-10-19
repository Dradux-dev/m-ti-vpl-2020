#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

namespace beuth {
  namespace thread {
    static std::mutex ioGuard;

    struct CountingSemaphore {
      public:
        using lock_type = std::unique_lock<std::mutex>;

        CountingSemaphore(std::ptrdiff_t init = 0)
          : count(init)
        {}

        CountingSemaphore(CountingSemaphore&& src) {
          count.store(src.count);
        }

        CountingSemaphore& operator=(CountingSemaphore&& src) = delete;

        void take( std::size_t n=1 ) {
          if ( n==0 ) {
            return;
          }

          lock_type l = lock_type(m);
          cv.wait(l, [&]{
            if ( count > 0 && count < (std::ptrdiff_t)n ) {
              n -= count;
              count = 0;
            }
            else if ( count >= (std::ptrdiff_t)n ) {
              count -= n;
              n = 0;
            }

            return n == 0;
          });
        }

        void wait(std::function<bool(std::ptrdiff_t)> predicate) {
          if (!predicate) {
            return;
          }

          lock_type l = lock_type(m);
          cv.wait(l, [&]() -> bool {
            return predicate(count);
          });
        }

        void give( std::size_t n=1 ) {
          if ( n==0 ) {
            return;
          }

          count += n;
          cv.notify_all();
        }

        // reduce the count without waiting for it
        void reduce(std::size_t n=1) {
          if ( n==0 ) {
            return;
          }

          count -= n;
          cv.notify_all();
        }

      protected:
        std::mutex m;
        std::condition_variable cv;
        std::atomic_ptrdiff_t count;
    };
  }
}
