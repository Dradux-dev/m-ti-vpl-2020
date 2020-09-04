#include "nosimd.h"

#include <matrix.hpp>

#include "randomizer.h"

NoSimd::NoSimd(const Config& config)
  : Variant<Config>(config)
{}

void NoSimd::run() {
  using Matrix = beuth::math::Matrix<float>;

  std::cout << "NoSimd(Benchmark)" << std::endl;

  MatrixRandomizer<float> randomizeMatrix;

  for (std::size_t size = config.minSize; size <= config.maxSize; ++size) {
    std::cout << "Performing benchmark for " << size << "x" << size << " matrix";
    std::cout.flush();

    Measurement measure;

    for (std::size_t test = 1; test <= config.iterations; ++test) {
      if ((test - 1) % (config.iterations / 20) == 0) {
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
