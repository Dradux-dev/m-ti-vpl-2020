#pragma once

#include "matrix.hpp"

namespace beuth {
  namespace math {
    template <typename T>
    Matrix<T> Step(const Matrix<T>& input);

    template <typename T>
    Matrix<T> Step(Matrix<T>&& input);

    template <typename T>
    Matrix<T> Step(const Matrix<T>& input) {
      return std::move(Step(Matrix<T>(input)));
    }

    template <typename T>
    Matrix<T> Step(Matrix<T>&& input) {
      for (std::size_t row = 0; row < input.rows; ++row) {
        for (std::size_t column = 0; column < input.columns; ++column) {
          input[row][column] = (input[row][column] >= 0) * input[row][column];
        }
      }

      return std::move(input);
    }
  }
}
