#include <iostream>
#include <chrono>
#include <limits>

#include <matrix.hpp>

#include "benchmark.h"
#include "config.h"
#include "evaluator.h"
#include "nosimd.h"

int main(int, char**)
{
  constexpr Config config(1, 75, 100);

  // Check setup
  static_assert (config.minSize > 0, "Matrix 0x0 is not allowed (min size)");
  static_assert (config.maxSize >= config.minSize, "Max has to be greater or equal to min");
  static_assert (config.iterations > 0, "Can't perform less than 1 test");

  Benchmark<Config> benchmark;
  benchmark.addVariant<NoSimd>(config);

  benchmark.run();
  std::cout << AvgEvaluator<Config>(benchmark);

  return 0;
}
