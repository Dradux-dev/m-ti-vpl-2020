#pragma once

#include <functional>

#include "../beuth-math/matrix.hpp"

namespace beuth {
  namespace ai {
    /// https://missinglink.ai/guides/neural-network-concepts/7-types-neural-network-activation-functions-right/
    /// https://arxiv.org/pdf/1710.05941v1.pdf
    /// https://dl.acm.org/doi/abs/10.1145/2964284.2984060
    /// https://www.researchgate.net/profile/Anand_Rangarajan/publication/2458489_Softmax_to_Softassign_Neural_Network_Algorithms_for_Combinatorial_Optimization/links/0deec5214b1cb9dbde000000.pdf
    /// https://link.springer.com/article/10.1007/s10916-018-0932-7
    /// https://www.sciencedirect.com/science/article/abs/pii/S1877750318302655
    /// https://arxiv.org/pdf/1505.00853.pdf
    /// https://de.wikipedia.org/wiki/Rectifier_(neuronale_Netzwerke)
    /// https://machinelearningmastery.com/rectified-linear-activation-function-for-deep-learning-neural-networks/
    /// https://medium.com/@himanshuxd/activation-functions-sigmoid-relu-leaky-relu-and-softmax-basics-for-neural-networks-and-deep-8d9c70eed91e
    /// https://en.wikipedia.org/wiki/Activation_function
    /// https://en.wikipedia.org/wiki/Softmax_function
    /// https://fabiensanglard.net/revisiting_the_pathtracer/SIMD_Tutorial.pdf
    /// https://www.youtube.com/watch?v=GwII1AJzKN4
    /// https://users.ece.cmu.edu/~franzf/teaching/slides-18-645-simd.pdf

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Layer
    //////////////////////////////////////////////////////////////////////////////////////////
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

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Neural Network
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    class NeuralNetwork {
      public:
        using Matrix = beuth::math::Matrix<TDataType>;
        using Layer = Layer<TDataType>;

        NeuralNetwork(const std::initializer_list<std::size_t>& l);

        Matrix processInputs(const Matrix& m);

      protected:
        std::size_t input = 0;
        std::size_t output = 0;
        Matrix outputs;
        std::vector<Layer> layers;
    };


    template <typename TDataType>
    NeuralNetwork<TDataType>::NeuralNetwork(const std::initializer_list<std::size_t>& l)
      : outputs(1,1)
    {
      std::size_t currentLayer = 0;
      std::size_t inputs = 0;
      for (std::size_t neurons : l) {
        if (currentLayer == 0) {
          this->input = neurons;
        }
        else {
          layers.emplace_back(neurons, inputs);
        }

        inputs = neurons;
        ++currentLayer;
      }

      output = *(l.end() - 1);
    }

    template <typename TDataType>
    typename NeuralNetwork<TDataType>::Matrix NeuralNetwork<TDataType>::processInputs(const typename NeuralNetwork<TDataType>::Matrix& inputs) {
      assert(inputs.getColumns() == input);

      Matrix result = inputs;
      for (Layer& layer : layers) {
        result = layer.feedForward(result);
      }

      outputs = std::move(result);
      return outputs;
    }
  }
}
