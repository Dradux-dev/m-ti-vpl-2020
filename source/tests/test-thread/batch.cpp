#include <catch2/catch.hpp>

#include <atomic>
#include <chrono>
#include <functional>

#include <batch.hpp>

template <typename TProcess>
static inline std::chrono::milliseconds measure(TProcess&& process) {
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
  process();
  std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

TEST_CASE("Batch", "[thread]") {
  using Batch = beuth::thread::Batch;

  beuth::thread::Threadpool pool(2);

  std::mutex finishedGuard;
  std::atomic_int finished{0};
  auto fn = [&] {
    std::lock_guard<std::mutex> guard(finishedGuard);
    ++finished;
  };

  auto alreadyFinished = [&](int amount) {
    return [&] {
      int finishedValue = 0;
      {
        std::lock_guard<std::mutex> guard(finishedGuard);
        finishedValue = finished;
      }

      REQUIRE(finishedValue >= amount);
      fn();
    };
  };

  SECTION("parallel execution") {
    Batch batch(pool, 2);

    batch.process(alreadyFinished(0));
    batch.process(alreadyFinished(0));
    batch.wait();
  }

  SECTION("blocking") {
    Batch batch(pool, 3);

    batch.process(alreadyFinished(0));
    batch.process(alreadyFinished(0));
    batch.process(alreadyFinished(1));

    batch.wait();
  }

  SECTION("wait") {
    constexpr int TEST_COUNT = 100;
    Batch batch(pool, TEST_COUNT);

    for(int i = 0; i < TEST_COUNT; ++i) {
      batch.process([&] {
        // Only to ensure that finished is less than TEST_COUNT before wait
        std::this_thread::sleep_for(std::chrono::milliseconds(4));

        // Increment finished
        fn();
      });
    }

    REQUIRE(finished < TEST_COUNT);
    batch.wait();
    REQUIRE(finished == TEST_COUNT);
  }
}
