#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>

#include <benchmark.hpp>
#include <evaluator.hpp>
#include <matrix-plain.hpp>

#include "config.h"
#include "nosimd.h"
#include "simd.h"

template <typename T>
bool exportData(const std::string& suffix, const T& t) {
  const std::string filename = "matrix-multiplication" + (suffix.empty() ? "" : std::string("-") + suffix) + ".dat";
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

int main(int, char**)
{
  BEUTH_PROFILING_CHROME_EXPORT();
  BEUTH_PROFILING_BEGIN_SESSION("profiling");
  //constexpr Config config(1, 5760, 250, 1);
  constexpr Config config(250, 250, 1, 1);

  // Check setup
  static_assert (config.minSize > 0, "Matrix 0x0 is not allowed (min size)");
  static_assert (config.maxSize >= config.minSize, "Max has to be greater or equal to min");
  static_assert (config.iterations > 0, "Can't perform less than 1 test");

  beuth::benchmark::Benchmark<Config> benchmark;
  benchmark.addVariant<NoSimd>(config);
  benchmark.addVariant<Simd>(config);

  benchmark.run();

  saveData(beuth::benchmark::MinEvaluator<Config>(benchmark));
  saveData(beuth::benchmark::AvgEvaluator<Config>(benchmark));
  saveData(beuth::benchmark::MaxEvaluator<Config>(benchmark));

  std::cout << "Benchmark data successfully saved" << std::endl;

  BEUTH_PROFILING_END_SESSION();
  return 0;
}
