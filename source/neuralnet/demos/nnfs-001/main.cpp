#include <iostream>

#include <matrix.hpp>

using Matrixf = beuth::math::Matrix<float>;

int main()
{
  Matrixf inputs(1, 4);
  inputs = {1, 2, 3, 4};
  std::cout << "inputs = [" << std::endl
            << inputs
            << "]" << std::endl;

  Matrixf weights(1,4);
  weights = {5, 10, 15, 20};

  Matrixf bias(1,1);
  bias = {7};

  Matrixf output = Matrixf::each_row(weights * Matrixf::Transpose(inputs)) + bias;
  std::cout << "outputs = [" << std::endl
            << output
            << "]" << std::endl;

  return 0;
}
