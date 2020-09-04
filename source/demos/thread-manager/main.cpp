#include <stdio.h>  // puts
#include <vector>
#include <thread>

// custom library

#include <thread_manager.hpp>

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

using namespace std;


int main(int, char**)
{
  using ThreadManager = beuth::thread::ThreadManager<Job,Result>;
  vector<Job> jobs = {Job(1,2), Job(0,4), Job(2,3), Job(5,5), Job(8,3)};

  ThreadManager m(4, [](Job&& job) -> Result {
    return Result(job());
  });

  vector<Result> result = m.processJobs(std::move(jobs));

  for (const Result &r : result)
  {
      printf("%d\n", r.result);
  }

  //[[maybe_unused]] int x = 5 + 8;
  //assert(x==13);
  return 0;
}
