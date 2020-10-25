#pragma once

#include <cmath>
#include "matrix-plain.hpp"

namespace beuth {
  namespace math {
    template <typename T>
    Matrix<T> LeakyReLU(const Matrix<T>& input);

    template <typename T>
    Matrix<T> LeakyReLU(Matrix<T>&& input);

    template <typename T>
    Matrix<T> ReLU(const Matrix<T>& input);

    template <typename T>
    Matrix<T> ReLU(Matrix<T>&& input);

    template <typename T>
    Matrix<T> Sigmoid(const Matrix<T>& input);

    template <typename T>
    Matrix<T> Sigmoid(Matrix<T>&& input);

    template <typename T>
    Matrix<T> Tanh(const Matrix<T>& input);

    template <typename T>
    Matrix<T> Tanh(Matrix<T>&& input);

    template <typename T>
    Matrix<T> LeakyReLU(const Matrix<T>& input) {
      return std::move(LeakyReLU(Matrix<T>(input)));
    }

    template <typename T>
    Matrix<T> LeakyReLU(Matrix<T>&& input) {
      for (std::size_t row = 0; row < input.rows; ++row) {
        for (std::size_t column = 0; column < input.columns; ++column) {
          input[row][column] = (input[row][column] <= 0) * 0.01 * input[row][column] + (input[row][column] > 0) * input[row][column];
        }
      }

      return std::move(input);
    }

    template <typename T>
    Matrix<T> ReLU(const Matrix<T>& input){
      return std::move(ReLU(Matrix<T>(input)));
    }

    template <typename T>
    Matrix<T> ReLU(Matrix<T>&& input) {
      for (std::size_t row = 0; row < input.rows; ++row) {
        for (std::size_t column = 0; column < input.columns; ++column) {
          input[row][column] = (input[row][column] > 0) * input[row][column];
        }
      }

      return std::move(input);
    }

    template <typename T>
    Matrix<T> Sigmoid(const Matrix<T>& input) {
      return std::move(Sigmoid(Matrix<T>(input)));
    }

    template <typename T>
    Matrix<T> Sigmoid(Matrix<T>&& input) {
      for (std::size_t row = 0; row < input.rows; ++row) {
        for (std::size_t column = 0; column < input.columns; ++column) {
          input[row][column] = 1 / (1 + std::exp(-input[row][column]));
        }
      }

      return std::move(input);
    }

    template <typename T>
    Matrix<T> Tanh(const Matrix<T>& input) {
      return std::move(Tanh(Matrix<T>(input)));
    }

    template <typename T>
    Matrix<T> Tanh(Matrix<T>&& input) {
      for (std::size_t row = 0; row < input.rows; ++row) {
        for (std::size_t column = 0; column < input.columns; ++column) {
          input[row][column] = std::tanh(input[row][column]);
        }
      }

      return std::move(input);
    }
  }
}
