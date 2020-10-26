#include <catch2/catch.hpp>

#include <threadpool.h>

TEST_CASE("Threadpool", "[thread]") {
  using Threadpool = beuth::thread::Threadpool;

  std::mutex finishedGuard;
  std::atomic_int finished{0};
  auto fn = [&] {
    std::lock_guard<std::mutex> guard(finishedGuard);
    ++finished;
  };

  auto alreadyFinished = [&](int amount) {
    return [&, amount] {
      int finishedValue = 0;
      {
        std::lock_guard<std::mutex> guard(finishedGuard);
        finishedValue = finished;
      }

      REQUIRE(finishedValue >= amount);
      fn();
    };
  };

  std::vector<std::future<void>> futures;

  Threadpool pool(2);

  SECTION("parallel execution") {
    futures.emplace_back(pool.process(alreadyFinished(0)));
    futures.emplace_back(pool.process(alreadyFinished(0)));

    for(std::future<void>& f : futures) {
      f.wait();
    }
  }

  SECTION("blocking") {
    futures.emplace_back(pool.process(alreadyFinished(0)));
    futures.emplace_back(pool.process(alreadyFinished(0)));
    futures.emplace_back(pool.process(alreadyFinished(1)));

    for(std::future<void>& f : futures) {
      f.wait();
    }
  }

  SECTION("process list") {
    constexpr std::size_t TASK_COUNT = 100;
    std::vector<Threadpool::Process> processList;
    for(std::size_t n = 0; n < TASK_COUNT; ++n) {
      processList.emplace_back(fn);
    }

    pool.process(std::move(processList));
    REQUIRE(finished == TASK_COUNT);
  }
}
