#include <catch2/catch.hpp>

#include <functional>
#include <queue>

#include <thread.h>

TEST_CASE("Thread", "[thread]") {
  using Counter = beuth::thread::CountingSemaphore;
  using Guard = std::lock_guard<std::mutex>;
  using Thread = beuth::thread::Thread;

  struct TaskQueue : public Thread::Parent {
      Thread thread;
      std::mutex queueGuard;
      std::queue<Thread::Process> queue;
      std::mutex futureGuard;
      std::vector<std::future<void>> futures;
      std::atomic_bool ready{false};
      Counter started;
      std::mutex queueSizeGuard;
      std::condition_variable queueSizeCond;


      inline TaskQueue()
        : thread(this),
          started(0)
      {
        // Wait until thread is ready for first task
        started.take();
      }

      inline ~TaskQueue() {
        thread.stop();
      }

      inline void add(Thread::Process&& process) {
        if (ready) {
          ready = false;
          Guard guard(futureGuard);
          futures.emplace_back(thread.wakeUp(process));
        }
        else {
          {
            Guard guard(queueGuard);
            queue.push(process);
          }
          queueSizeCond.notify_all();
        }
      }

      inline void wait() {
        // Wait for queue to be cleared
        {
          std::unique_lock<std::mutex> l(queueSizeGuard);
          queueSizeCond.wait(l, [&]() {
            Guard guard(queueGuard);
            return queue.size() == 0;
          });
        }

        Guard guard(futureGuard);
        for(std::future<void>& f : futures) {
          f.wait();
        }

        futures.clear();
      }

    protected:
      inline void threadIsReady(Thread* t) override {
        started.give();

        std::size_t size = 0;
        {
          Guard guard(queueGuard);
          size = queue.size();
        }
        if (size > 0) {
          Thread::Process process = nullptr;
          {
            Guard guard(queueGuard);
            process = queue.front();
            queue.pop();
          }
          queueSizeCond.notify_all();

          Guard guard(futureGuard);
          futures.emplace_back(t->wakeUp(process));
        }
        else {
          ready = true;
        }
      }
  };

  TaskQueue queue;

  SECTION("busy") {
    queue.add([] {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    REQUIRE(queue.thread.isBusy());
    queue.wait();
    REQUIRE(!queue.thread.isBusy());
  }

  SECTION("available") {
    queue.add([] {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    REQUIRE(!queue.thread.isAvailable());
    queue.wait();
    REQUIRE(queue.thread.isAvailable());
  }

  SECTION("ready") {
    std::mutex counterGuard;
    std::atomic_int counter{0};

    auto fn = [&]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));

      Guard guard(counterGuard);
      ++counter;
    };

    queue.add(fn);
    queue.add(fn);
    queue.add(fn);

    queue.wait();

    REQUIRE(counter == 3);
  }
}
