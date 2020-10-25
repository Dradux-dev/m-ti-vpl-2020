#include <catch2/catch.hpp>

#include <thread-manager.hpp>

TEST_CASE("ThreadManager (Job based)", "[thread]") {
  using Guard = std::lock_guard<std::mutex>;
  using Job = std::pair<int, int>;
  using ThreadManager = beuth::thread::ThreadManager<Job, void>;
  using Threadpool = beuth::thread::Threadpool;

  Threadpool pool(2);
  std::mutex resultGuard;
  std::vector<int> result;

  SECTION("job list") {
    std::vector<Job> jobs = {
      std::make_pair(3, 2),
      std::make_pair(4, 4)
    };

    ThreadManager manager(pool, std::move(jobs), [&](Job&& job) {
      Guard guard(resultGuard);
      result.emplace_back(std::pow(job.first,job.second));
    });

    manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("job assignment") {
    ThreadManager manager(pool, [&](Job&& job) {
      Guard guard(resultGuard);
      result.emplace_back(std::pow(job.first,job.second));
    });

    manager.addJob(std::make_pair(3, 2));
    manager.addJob(std::make_pair(4, 4));

    manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("blocking") {
    ThreadManager manager(pool, [&](Job&& job) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      Guard guard(resultGuard);
      result.emplace_back(std::pow(job.first,job.second));
    });

    manager.addJob(std::make_pair(3, 2));
    manager.addJob(std::make_pair(4, 4));
    manager.addJob(std::make_pair(2, 5));

    manager.execute();

    REQUIRE(result.size() == 3);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 32) != result.end());
  }
}
