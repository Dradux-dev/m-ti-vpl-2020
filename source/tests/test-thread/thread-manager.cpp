#include <catch2/catch.hpp>

#include <thread-manager.hpp>

TEST_CASE("ThreadManager", "[thread]") {
  using Guard = std::lock_guard<std::mutex>;
  using ThreadManager = beuth::thread::ThreadManager<void, void>;
  using Threadpool = beuth::thread::Threadpool;

  Threadpool pool(2);
  std::mutex resultGuard;
  std::vector<int> result;

  auto generator = [&](int base, int exponent, int sleep = 0) {
    return [base, exponent, sleep, &resultGuard, &result]() {
      if (sleep > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
      }

      Guard guard(resultGuard);
      int value = std::pow<int>(base, exponent);
      result.emplace_back(value);
    };
  };

  SECTION("job list") {
    std::vector<ThreadManager::Process> jobs = {
      generator(3, 2),
      generator(4, 4)
    };

    ThreadManager manager(pool, std::move(jobs));

    manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("job assignment") {
    ThreadManager manager(pool);

    manager.addJob(generator(3, 2));
    manager.addJob(generator(4, 4));

    manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("blocking") {
    ThreadManager manager(pool);

    manager.addJob(generator(3, 2, 100));
    manager.addJob(generator(4, 4, 100));
    manager.addJob(generator(2, 5, 100));

    manager.execute();

    REQUIRE(result.size() == 3);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 32) != result.end());
  }
}
