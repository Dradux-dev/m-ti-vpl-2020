#pragma once

#include <functional>

#include "../beuth-math/matrix.hpp"

namespace beuth {
  namespace ai {
    template <typename TDataType>
    class Layer {
      public:
        using Matrix = beuth::math::Matrix<TDataType>;
        using Activation = std::function<Matrix(Matrix&&)>;

        Layer(std::size_t neurons, std::size_t inputs, Activation activation = nullptr);

        Matrix feedForward(Matrix&&) const;
        Matrix feedForward(const Matrix&) const;

      protected:
        Matrix weights;
        Matrix biases;
        Activation activation;
    };


    template <typename TDataType>
    Layer<TDataType>::Layer(std::size_t neurons, std::size_t inputs, Layer<TDataType>::Activation activation)
      : weights(inputs, neurons),
        biases(1, neurons),
        activation(activation)
    {}

    template <typename TDataType>
    typename Layer<TDataType>::Matrix Layer<TDataType>::feedForward(typename Layer<TDataType>::Matrix&& inputs) const {
      const Layer<TDataType>::Matrix& cinputs = inputs;
      return std::move(feedForward(cinputs));
    }

    template <typename TDataType>
    typename Layer<TDataType>::Matrix Layer<TDataType>::feedForward(const typename Layer<TDataType>::Matrix& inputs) const {
      if (activation) {
        return std::move(activation(typename Matrix::each_row(inputs * weights) + biases));
      }

      return std::move(typename Matrix::each_row(inputs * weights) + biases);
    }
  }
}
