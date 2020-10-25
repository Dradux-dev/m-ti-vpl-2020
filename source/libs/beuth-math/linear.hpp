#pragma once

#include "matrix-plain.hpp"

namespace beuth {
  namespace math {
    template <typename T>
    Matrix<T> Linear(const Matrix<T>& input);

    template <typename T>
    Matrix<T> Linear(Matrix<T>&& input);

    template <typename T>
    Matrix<T> Linear(const Matrix<T>& input) {
      return std::move(Matrix<T>(input));
    }

    template <typename T>
    Matrix<T> Linear(Matrix<T>&& input) {
      return std::move(input);
    }
  }
}
