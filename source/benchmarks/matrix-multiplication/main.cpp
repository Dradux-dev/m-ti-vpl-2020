#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>

#include <matrix.hpp>

#include "benchmark.h"
#include "config.h"
#include "evaluator.h"
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
inline bool saveData(const MinEvaluator<TConfig>& min) {
  return exportData("min", min);
}

template <typename TConfig>
inline bool saveData(const AvgEvaluator<TConfig>& avg) {
  return exportData("avg", avg);
}

template <typename TConfig>
inline bool saveData(const MaxEvaluator<TConfig>& max) {
  return exportData("max", max);
}

int main(int, char**)
{
  constexpr Config config(1, 5760, 250, 1);

  // Check setup
  static_assert (config.minSize > 0, "Matrix 0x0 is not allowed (min size)");
  static_assert (config.maxSize >= config.minSize, "Max has to be greater or equal to min");
  static_assert (config.iterations > 0, "Can't perform less than 1 test");

  Benchmark<Config> benchmark;
  benchmark.addVariant<NoSimd>(config);
  benchmark.addVariant<Simd>(config);

  benchmark.run();

  saveData(MinEvaluator<Config>(benchmark));
  saveData(AvgEvaluator<Config>(benchmark));
  saveData(MaxEvaluator<Config>(benchmark));

  std::cout << "Benchmark data successfully saved" << std::endl;

  return 0;
}
