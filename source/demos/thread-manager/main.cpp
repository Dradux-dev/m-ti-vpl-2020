#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
#include <thread>
#include <vector>

// custom library

#include <thread-manager.hpp>

/*
struct Job
{
  std::vector<float> imageData; 	// Bildinformationen
  std::vector<float> expectation; // erwarteter Ergebnisvektor
  enum Mode {training, test}; 	// Modusvorgabe
};
*/

struct Job
{
    int lhs;
    int rhs;

    inline Job(int lhs, int rhs)
        : lhs(lhs),
          rhs(rhs)
    {}

    Job() = default;
    Job(const Job&) = default;
    Job(Job&&) = default;

    inline int operator()()
    {
        return lhs + rhs;
    }

    Job& operator=(Job&& job) {
        lhs = job.lhs;
        rhs = job.rhs;
        return *this;
    }
};

struct Result
{
    int result;
    inline explicit Result(int result)
        : result(result)
    {}

    Result() = default;
    Result(const Result&) = default;
    Result(Result&&) = default;

    Result& operator=(const Result& r) {
        result = r.result;
        return *this;
    }
};

struct Average {
    float value;
    std::size_t count;

    inline Average()
      : value(0.0f),
        count(0)
    {}

    [[nodiscard]] inline operator float() const noexcept {
      return value;
    }
};

Average& operator<<(Average& a, float v) {
  ++a.count;
  const float proportion = 1.0f / a.count;
  a.value = (v * proportion) + ((1-proportion) * a.value);
  return a;
}

using namespace std;

int main(int, char**)
{
  using Threadpool = beuth::thread::Threadpool;
  using ThreadManager = beuth::thread::ThreadManager<Job,Result>;

  constexpr std::size_t JOB_COUNT = 64*64;
  constexpr std::size_t RUN_COUNT = 100000;
  constexpr std::size_t PRECISION = 0;
  const std::size_t DIVIDER = (RUN_COUNT / 100) * std::pow(10, PRECISION);
  constexpr bool SHOW_PROGRESS = true;
  constexpr bool SHOW_RESULTS = false;

  Threadpool pool(20);
  std::random_device rd;
  std::uniform_int_distribution<int> d(0, 10);
  Average avg;

  for (std::size_t run = 0; run < RUN_COUNT; ++run) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    vector<Job> jobs;
    jobs.reserve(JOB_COUNT);

    for(std::size_t n = 0; n < JOB_COUNT; ++n) {
      jobs.emplace_back(d(rd), d(rd));
    }


  //  vector<Job> jobs = {
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3),
  //    Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3)
  //  };


    ThreadManager m(pool, std::move(jobs), [](Job&& job) -> Result {
//      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      return Result(job());
    });

    vector<Result> result = m.execute();

    if (SHOW_RESULTS) {
      for (const Result &r : result)
      {
          std::cout << r.result << " ";
      }

      if (SHOW_PROGRESS) {
        std::cout << std::endl;
      }
      else {
        std::cout <<  "| ";
      }
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    avg << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() * std::pow(10.0f, -6.0f);

    if (SHOW_PROGRESS) {
      if ((run % std::max(DIVIDER, std::size_t{1})) == 0) {
        std::cout << run << " / " << RUN_COUNT << " (" << std::fixed << std::setprecision(std::max(std::size_t{0}, PRECISION)) << (static_cast<float>(run) / static_cast<float>(RUN_COUNT))*100 << "%, remaining " << avg*(RUN_COUNT-run) << "s)" << std::endl;
      }
    }
  }

  std::cout << std::endl;

  return 0;
}
