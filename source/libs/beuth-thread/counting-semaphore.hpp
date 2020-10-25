#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

namespace beuth {
  namespace thread {
    /**
     * @brief Counting semaphore
     *
     * A semaphore, which is based on a counter.
     *
     * @author Tarek Schwarzinger
     */
    struct CountingSemaphore {
      public:
        /// Lock type definition
        using lock_type = std::unique_lock<std::mutex>;

        /**
         * @brief Constructor
         *
         * Constructs the counting semaphore and initializes the counter.
         *
         * @param init The initialization value for the counter. By default
         *             the counter will be initialized with zero.
         *
         * @authors Christian Haß, Tarek Schwarzinger
         */
        CountingSemaphore(std::ptrdiff_t init = 0)
          : count(init)
        {}

        /**
         * @brief Constructor
         *
         * Moves the counter of one semaphore to another one.
         *
         * @param src Source semaphore that contains the counter, that shall be moved.
         *
         * @authors Christian Haß, Tarek Schwarzinger
         */
        CountingSemaphore(CountingSemaphore&& src) {
          count.store(src.count);
        }

        CountingSemaphore& operator=(CountingSemaphore&& src) = delete;

        /**
         * @brief take
         *
         * Takes values from the counter as soon as values are available. Otherwise
         * this function waits for availability of the counter.
         *
         * @param n The amount that shall be taken from the counter.
         *
         * @attention This is a blocking call, that will wait until the full amount n could
         *            be taken from the counter.
         *
         * @test "CountingSemaphore/instant" proves, that the take() function ends
         *       immediately, if the required count is already available.
         * @test "CountingSemaphore/blocking" proves, that the take() function is
         *       blocking until the required count is available.
         * @test "CountingSemaphore/take multiple" proves, that the take() function is
         *       blocking until the full amount is available. It also proves, that
         *       the take function is reserving count as soon as it gets
         *       available, even if only partially.
         * @test "CountingSemaphore/remaining" proves, that the take() function is
         *        only reserving the required amount and not more.
         *
         * @authors Christian Haß, Tarek Schwarzinger
         */
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

        /**
         * @brief wait
         *
         * Waits for a certain counter predicate to be fullfilled.
         *
         * @param predicate The predicate that needs to be fullfilled.
         *
         * @attention This is a blocking call, that will wait until the predicate
         *            is fullfilled.
         *
         * @test "CountingSemaphore/wait" proves that the wait function is blocking
         *       until the required count condition is met, without changing the
         *       count value.
         *
         * @authors Christian Haß, Tarek Schwarzinger
         */
        void wait(std::function<bool(std::ptrdiff_t)> predicate) {
          if (!predicate) {
            return;
          }

          lock_type l = lock_type(m);
          cv.wait(l, [&]() -> bool {
            return predicate(count);
          });
        }

        /**
         * @brief give
         *
         * Gives an amount to the counter and notifies all waiting threads.
         *
         * @param n The amount that shall be added to the counter.
         *
         * @authors Christian Haß, Tarek Schwarzinger
         */
        void give( std::size_t n=1 ) {
          if ( n==0 ) {
            return;
          }

          {
            lock_type l = lock_type(m);
            count += n;
          }
          cv.notify_all();
        }

        /**
         * @brief operator std::ptrdiff_t
         *
         * @return Returns the current value of the counter.
         *
         * @author Tarek Schwarzinger
         */
        inline operator std::ptrdiff_t() const {
          return count;
        }

      protected:
        /// Mutex to guard the counter
        std::mutex m;

        /// Condition variable to wait for
        std::condition_variable cv;

        /// The counter
        std::atomic_ptrdiff_t count;
    };
  }
}
