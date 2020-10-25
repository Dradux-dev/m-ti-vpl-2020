#include <catch2/catch.hpp>

#include <batch.hpp>
#include <counting-semaphore.hpp>

TEST_CASE("CountingSemaphore", "[thread]") {
  using Batch = beuth::thread::Batch;
  using Counter = beuth::thread::CountingSemaphore;
  using Threadpool = beuth::thread::Threadpool;

  Threadpool pool(2);
  Counter counter;

  auto give = [&] {
    counter.give();
  };

  auto take = [&] {
    counter.take();
  };

  SECTION("Instant") {
    give();
    take();
    REQUIRE(counter == 0);
  }

  SECTION("Blocking") {
    Batch batch(pool, 2);

    batch.process([&] {
      take();
      REQUIRE(counter == 0);
    });

    batch.process([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();
    });

    batch.wait();
    REQUIRE(counter == 0);
  }

  SECTION("Take Multiple") {
    Batch batch(pool, 3);

    batch.process([&] {
      counter.take(2);
      REQUIRE(counter == 0);
    });

    batch.process([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();
    });

    batch.process([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();
    });

    batch.wait();
    REQUIRE(counter == 0);
  }

  SECTION("Remaining") {
    give();
    give();

    take();
    REQUIRE(counter == 1);
  }

  SECTION("Wait") {
    Batch batch(pool, 2);

    batch.process([&] {
      counter.wait([](std::ptrdiff_t value) -> bool {
        return value == 3;
      });
    });

    batch.process([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();

      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();

      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      give();
    });

    batch.wait();
    REQUIRE(counter == 3);
  }
}
