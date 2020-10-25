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
}
