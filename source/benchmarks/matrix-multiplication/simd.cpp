#include "simd.h"

#include <algorithm>
#define BEUTH_MATH_USE_SIMD
#include <matrix-plain.hpp>
#include <profiling.h>

#include "randomizer.h"

Simd::Simd(const Config& config)
  : Variant<Config>(config, "SIMD")
{}

void Simd::run() {
  BEUTH_PROFILING_FUNCTION("benchmark,variant,execution");
  using Matrix = beuth::math::Matrix<float>;

  std::cout << "Simd(Benchmark)" << std::endl;

  MatrixRandomizer<float> randomizeMatrix;

  auto getStep = [&](std::size_t size) {
    return std::max(std::min(config.maxSize - size, config.stepSize), std::size_t(1));
  };

  for (std::size_t size = config.minSize; size <= config.maxSize; size += getStep(size)) {
    std::cout << "Performing benchmark for " << size << "x" << size << " matrix";
    std::cout.flush();

    Measurement measure;

    for (std::size_t test = 1; test <= config.iterations; ++test) {
      if ((config.iterations / 20) == 0 || (test - 1) % (config.iterations / 20) == 0) {
        std::cout << "." << std::flush;
      }

      Matrix a = Matrix(size, size);
      Matrix b = Matrix(size, size);
      randomizeMatrix(a);
      randomizeMatrix(b);

      measure.start();
      Matrix r = a * b;
      measure.stop();
    }

    measurements.push_back(std::move(measure));
    std::cout << MeasurementConverter(measure, 1000) << std::endl;
  }
}
