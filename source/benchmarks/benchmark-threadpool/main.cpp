#include <chrono>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>

#include <benchmark.hpp>
#include <evaluator.hpp>

#include "async.h"
#include "config.h"
#include "pool.h"
#include "single-pool.h"

template <typename T>
bool exportData(const std::string& suffix, const T& t) {
  const std::string filename = "benchmark-threadpool" + (suffix.empty() ? "" : std::string("-") + suffix) + ".dat";
  std::ofstream dat(filename);
  if (!dat.is_open()) {
    std::cerr << "Can not open \"" << filename << "\"" << std::endl;
    return false;
  }

  dat << t;
  dat.close();
  return true;
}

template <typename TConfig>
inline bool saveData(const beuth::benchmark::MinEvaluator<TConfig>& min) {
  return exportData("min", min);
}

template <typename TConfig>
inline bool saveData(const beuth::benchmark::AvgEvaluator<TConfig>& avg) {
  return exportData("avg", avg);
}

template <typename TConfig>
inline bool saveData(const beuth::benchmark::MaxEvaluator<TConfig>& max) {
  return exportData("max", max);
}

int main()
{
  // 1 to 1000 Tasks (sleep 5ms) with 10 Iterations: 9601 seconds
  std::mutex m;
  Config config = {
    .tasks = {
      .min = 200,
      .max = 200
    },
    .task = [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    },
    .iterations = 1
  };

  beuth::benchmark::Benchmark<Config> benchmark;
  //benchmark.addVariant<Pool>(config);
  benchmark.addVariant<Async>(config);
  benchmark.addVariant<SinglePool>(config);

  benchmark.run();

  saveData(beuth::benchmark::MinEvaluator<Config>(benchmark));
  saveData(beuth::benchmark::AvgEvaluator<Config>(benchmark));
  saveData(beuth::benchmark::MaxEvaluator<Config>(benchmark));

  return 0;
}
