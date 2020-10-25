#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <random>
#include <vector>

using namespace std;

int main()
{
  constexpr std::size_t ASYNC_COUNT = 4;
  constexpr std::size_t TASK_COUNT = 4000;

  std::random_device rdevice;
  std::uniform_int_distribution<int> d(0,2000);

  std::mutex outMutex;

  for (std::size_t n = 0; n < 10; ++n) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << ".";
  }
  std::cout << std::endl;

  {
    std::lock_guard<std::mutex> guard(outMutex);
    std::cout << "Reserving future memory" << std::endl;
  }
  std::vector<std::future<void>> futures;
  futures.reserve(ASYNC_COUNT);

  {
    std::lock_guard<std::mutex> guard(outMutex);
    std::cout << "Creating futures" << std::endl;
  }
  for (std::size_t n = 0; n < TASK_COUNT; ++n) {
    if (futures.size() == ASYNC_COUNT) {
      for (std::future<void>& f : futures) {
        f.wait();
      }
      futures.clear();
    }

    std::size_t sleepTime = d(rdevice);
    futures.emplace_back(std::async([sleepTime, n, &outMutex] {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
      std::lock_guard<std::mutex> guard(outMutex);
      std::cout << "Task #" << n << " finished" << std::endl;
    }));
  }

  {
    std::lock_guard<std::mutex> guard(outMutex);
    std::cout << "Waiting for futures" << std::endl;
  }

  for (std::future<void>& f : futures) {
    f.wait();
  }

  {
    std::lock_guard<std::mutex> guard(outMutex);
    std::cout << "Done" << std::endl;
  }
  return 0;
}
