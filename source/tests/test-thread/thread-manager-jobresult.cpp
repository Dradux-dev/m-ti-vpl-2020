#include <catch2/catch.hpp>

#include <thread-manager.hpp>

TEST_CASE("ThreadManager (Result based)", "[thread]") {
  using Job = std::pair<int, int>;
  using Result = int;
  using ThreadManager = beuth::thread::ThreadManager<Job, Result>;
  using Threadpool = beuth::thread::Threadpool;

  Threadpool pool(2);

  SECTION("job list") {
    std::vector<Job> jobs = {
      std::make_pair(3, 2),
      std::make_pair(4, 4)
    };

    ThreadManager manager(pool, std::move(jobs), [&](Job&& job) -> Result {
      return std::pow(job.first,job.second);
    });

    std::vector<Result> result = manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("job assignment") {
    ThreadManager manager(pool, [&](Job&& job) -> Result {
      return std::pow(job.first,job.second);
    });

    manager.addJob(std::make_pair(3, 2));
    manager.addJob(std::make_pair(4, 4));

    std::vector<Result> result = manager.execute();

    REQUIRE(result.size() == 2);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
  }

  SECTION("blocking") {
    ThreadManager manager(pool, [&](Job&& job) -> Result {
      return std::pow(job.first,job.second);
    });

    manager.addJob(std::make_pair(3, 2));
    manager.addJob(std::make_pair(4, 4));
    manager.addJob(std::make_pair(2, 5));

    std::vector<Result> result = manager.execute();

    REQUIRE(result.size() == 3);
    REQUIRE(std::find(result.begin(), result.end(), 9) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 256) != result.end());
    REQUIRE(std::find(result.begin(), result.end(), 32) != result.end());
  }
}
