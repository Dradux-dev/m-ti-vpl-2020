#pragma once

#include <limits>

#include <matrix.hpp>

struct RandomNumberGenerator {
    static int Generate(int min, int max) {
      return (static_cast<int>(Instance()) % max) + min;
    }

    static double Generate(double min, double max) {
      return (static_cast<double>(Instance()) * max) + min;
    }

    operator int() const {
      return rand();
    }

    operator double() const {
      return static_cast<double>(rand()) / static_cast<double>(std::numeric_limits<int>::max());
    }

  protected:
    int generate() {
      return rand();
    }

  private:
    static RandomNumberGenerator& Instance() {
      static RandomNumberGenerator rng;
      return rng;
    }

    inline RandomNumberGenerator() {
      srand(time(NULL));
    }
};

template <typename T>
struct MatrixRandomizer {
    void operator ()(beuth::math::Matrix<T>& m) {
      for (std::size_t row = 0; row < m.getRows(); ++row) {
        for (std::size_t col = 0; col < m.getColumns(); ++col) {
          m[row][col] = RandomNumberGenerator::Generate(0.0, 1.0);
        }
      }
    }
};
