#include <iostream>

#include <matrix.hpp>
#include <neuralnetwork.h>

template <typename T>
class ConfigurableLayer : public beuth::ai::Layer<T> {
  public:
    using Matrix = typename beuth::ai::Layer<T>::Matrix;

    ConfigurableLayer(std::size_t neurons, std::size_t inputs)
      : beuth::ai::Layer<T>(neurons, inputs)
    {}

    void setWeight(const Matrix& weights) {
      this->weights = weights;
    }

    void setBiases(const Matrix& biases) {
      this->biases = biases;
    }
};

using Matrix = beuth::math::Matrix<float>;
using Layer = ConfigurableLayer<float>;



int main()
{
  Matrix inputs(1, 4);
  inputs = {1, 2, 3, 4};
  std::cout << "inputs = [" << std::endl
            << inputs
            << "]" << std::endl;

  Matrix weights(4,2);
  weights = {5, 1,
             10, 2,
             15, 4,
             20, 8};

  Matrix bias(1,2);
  bias = {7, 9};

  Matrix output = Matrix::each_row(inputs * weights) + bias;
  std::cout << "outputs = [" << std::endl
            << output
            << "]" << std::endl;

  // Testing basic layer implementation
  Layer testLayer(2, 4);
  testLayer.setWeight(weights);
  testLayer.setBiases(bias);
  std::cout << "layer = [" << std::endl
            << testLayer.feedForward(inputs)
            << "]" << std::endl;

  return 0;
}
