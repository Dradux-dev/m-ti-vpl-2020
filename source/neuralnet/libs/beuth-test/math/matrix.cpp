#include <catch2/catch.hpp>

#include <beuth-math/matrix.hpp>

template <typename T>
using Matrix = beuth::math::Matrix<T>;

TEST_CASE("Matrix", "[math]") {
  Matrix<std::uint8_t> m8(3, 3);

  SECTION("Construction") {
    REQUIRE(m8.getRows() == 3);
    REQUIRE(m8.getColumns() == 3);
  }

  SECTION("Setting and getting elements") {
    for (std::size_t x = 0; x < 3; ++x) {
      for (std::size_t y = 0; y < 3; ++y) {
        std::size_t value = y * 3 + x + 1;
        m8[y][x] = value;
      }
    }

    REQUIRE(m8[0][0] == 1);
    REQUIRE(m8[0][1] == 2);
    REQUIRE(m8[0][2] == 3);

    REQUIRE(m8[1][0] == 4);
    REQUIRE(m8[1][1] == 5);
    REQUIRE(m8[1][2] == 6);

    REQUIRE(m8[2][0] == 7);
    REQUIRE(m8[2][1] == 8);
    REQUIRE(m8[2][2] == 9);
  }

  SECTION("Addition") {
    Matrix<std::uint8_t> a(2,2);
    Matrix<std::uint8_t> b(2,2);

    a[0][0] = 2;
    a[0][1] = 5;
    a[1][0] = 3;
    a[1][1] = -4;

    b[0][0] = 7;
    b[0][1] = -2;
    b[1][0] = 1;
    b[1][1] = 4;

    Matrix<std::uint8_t> r = a + b;
    REQUIRE(r[0][0] == 9);
    REQUIRE(r[0][1] == 3);
    REQUIRE(r[1][0] == 4);
    REQUIRE(r[1][1] == 0);
  }
}
