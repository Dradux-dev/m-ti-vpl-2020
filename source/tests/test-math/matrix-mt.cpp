#include <catch2/catch.hpp>

#include <matrix-mt.hpp>

template <typename T>
using Matrix = beuth::math::concurrent::Matrix<T>;

using Threadpool = beuth::thread::Threadpool;

TEST_CASE("Matrix Multi-Threading", "[math]") {
  using DataType = std::uint32_t;

  Threadpool pool(4);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Threadpool created ... " << std::endl;

  Matrix<DataType> m(pool, 3, 3);
  for (std::size_t x = 0; x < 3; ++x) {
    for (std::size_t y = 0; y < 3; ++y) {
      std::size_t value = y * 3 + x + 1;
      m[y][x] = value;
    }
  }

  SECTION("Construction") {
    REQUIRE(m.getRows() == 3);
    REQUIRE(m.getColumns() == 3);
  }

  SECTION("Setting and getting elements") {
    REQUIRE(m[0][0] == 1);
    REQUIRE(m[0][1] == 2);
    REQUIRE(m[0][2] == 3);

    REQUIRE(m[1][0] == 4);
    REQUIRE(m[1][1] == 5);
    REQUIRE(m[1][2] == 6);

    REQUIRE(m[2][0] == 7);
    REQUIRE(m[2][1] == 8);
    REQUIRE(m[2][2] == 9);
  }

  SECTION("Addition") {
    Matrix<DataType> a(pool, 2,2);
    Matrix<DataType> b(pool, 2,2);

    a[0][0] = 2;
    a[0][1] = 5;
    a[1][0] = 3;
    a[1][1] = -4;

    b[0][0] = 7;
    b[0][1] = -2;
    b[1][0] = 1;
    b[1][1] = 4;

    std::cout << "Adding soon ... " << std::endl;
    Matrix<DataType> r = a + b;
    std::cout << "Adding finished ..." << std::endl;

    REQUIRE(r[0][0] == 9);
    REQUIRE(r[0][1] == 3);
    REQUIRE(r[1][0] == 4);
    REQUIRE(r[1][1] == 0);
  }

  SECTION("Multiplication") {
    Matrix<DataType> a(pool, 1,3);
    Matrix<DataType> b(pool, 3,1);

    a[0][0] = 1;
    a[0][1] = 2;
    a[0][2] = 3;

    b[0][0] = 4;
    b[1][0] = 5;
    b[2][0] = 6;

    // r = a * b
    {
      std::cout << "a * b soon ... " << std::endl;
      Matrix<DataType> r = a * b;
      std::cout << "a * b finished ... " << std::endl;
      REQUIRE(r.getRows() == 1);
      REQUIRE(r.getColumns() == 1);
      REQUIRE(r[0][0] == 32);
    }

    // r = b * a
    {
      std::cout << "b * a soon ... " << std::endl;
      Matrix<DataType> r = b * a;
      std::cout << "b * a finished ... " << std::endl;
      REQUIRE(r.getRows() == 3);
      REQUIRE(r.getColumns() == 3);

      REQUIRE(r[0][0] == 4);
      REQUIRE(r[0][1] == 8);
      REQUIRE(r[0][2] == 12);

      REQUIRE(r[1][0] == 5);
      REQUIRE(r[1][1] == 10);
      REQUIRE(r[1][2] == 15);

      REQUIRE(r[2][0] == 6);
      REQUIRE(r[2][1] == 12);
      REQUIRE(r[2][2] == 18);
    }
  }

  SECTION("Each Row: Addition") {
    Matrix<DataType> row = Matrix<DataType>(pool, 1, 3);
    row[0][0] = 1;
    row[0][1] = 1;
    row[0][2] = 1;

    std::cout << "Each row adding soon ..." << std::endl;
    Matrix<DataType> r = Matrix<DataType>::each_row(m) + row;
    std::cout << "Each row adding finished ..." << std::endl;
    REQUIRE(r.getRows() == 3);
    REQUIRE(r.getColumns() == 3);

    REQUIRE(r[0][0] == 2);
    REQUIRE(r[0][1] == 3);
    REQUIRE(r[0][2] == 4);

    REQUIRE(r[1][0] == 5);
    REQUIRE(r[1][1] == 6);
    REQUIRE(r[1][2] == 7);

    REQUIRE(r[2][0] == 8);
    REQUIRE(r[2][1] == 9);
    REQUIRE(r[2][2] == 10);
  }

  SECTION("Each Column: Addition") {
    Matrix<DataType> row = Matrix<DataType>(pool, 3, 1);
    row[0][0] = 1;
    row[1][0] = 1;
    row[2][0] = 1;

    std::cout << "Each column adding soon ..." << std::endl;
    Matrix<DataType> r = Matrix<DataType>::each_column(m) + row;
    std::cout << "Each column adding finished ..." << std::endl;
    REQUIRE(r.getRows() == 3);
    REQUIRE(r.getColumns() == 3);

    REQUIRE(r[0][0] == 2);
    REQUIRE(r[1][0] == 5);
    REQUIRE(r[2][0] == 8);

    REQUIRE(r[0][1] == 3);
    REQUIRE(r[1][1] == 6);
    REQUIRE(r[2][1] == 9);

    REQUIRE(r[0][2] == 4);
    REQUIRE(r[1][2] == 7);
    REQUIRE(r[2][2] == 10);
  }
}
