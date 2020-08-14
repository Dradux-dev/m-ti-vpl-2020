#pragma once

#include <assert.h>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <vector>

namespace beuth {
  namespace math {
    template <typename TDataType>
    class Matrix {
      public:
        friend struct row_index;
        friend struct const_row_index;
        friend struct column_index;
        friend struct const_column_index;
        friend struct each_row;
        friend struct each_column;

        template <typename T>
        friend Matrix<T> operator+(const Matrix<T>&, const Matrix<T>&);

        template <typename T>
        friend Matrix<T> operator+(const Matrix<T>& lhs, const typename Matrix<T>::each_row& rhs);

        template <typename T>
        friend Matrix<T> operator+(const typename Matrix<T>::each_row& lhs, const Matrix<T>& rhs);

        template <typename T>
        friend Matrix<T> operator+(const Matrix<T>& lhs, const typename Matrix<T>::each_column& rhs);

        template <typename T>
        friend Matrix<T> operator+(const typename Matrix<T>::each_column& lhs, const Matrix<T>& rhs);

        template <typename T>
        friend Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);

        template <typename T>
        friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

        struct row_index {
            Matrix<TDataType>& m;
            std::size_t row;

            row_index(Matrix<TDataType>& m, std::size_t row);
            row_index(row_index&&) = default;

            TDataType& operator[](std::size_t column);

            operator Matrix<TDataType>() const;
        };

        struct const_row_index {
            const Matrix<TDataType>& m;
            std::size_t row;

            const_row_index(const Matrix<TDataType>& m, std::size_t row);
            const_row_index(const_row_index&&) = default;

            const TDataType& operator[](std::size_t column) const;

            operator Matrix<TDataType>() const;
        };

        struct column_index {
            Matrix<TDataType>& m;
            std::size_t column;

            column_index(Matrix<TDataType>& m, std::size_t column);
            column_index(column_index&&) = default;

            TDataType& operator[](std::size_t row);

            operator Matrix<TDataType>() const;
        };

        struct const_column_index {
            const Matrix<TDataType>& m;
            std::size_t column;

            const_column_index(const Matrix<TDataType>& m, std::size_t column);
            const_column_index(const_column_index&&) = default;

            const TDataType& operator[](std::size_t column) const;

            operator Matrix<TDataType>() const;
        };

        struct each_row {
            Matrix<TDataType> m;

            each_row(const Matrix<TDataType>& m);
            each_row(Matrix<TDataType>&& m);
        };

        struct each_column {
            Matrix<TDataType> m;

            each_column(const Matrix<TDataType>& m);
            each_column(Matrix<TDataType>&& m);
        };

        Matrix(std::size_t rows, std::size_t columns);
        Matrix(const Matrix&) = default;
        Matrix(Matrix&&) = default;

        std::size_t getRows() const;
        std::size_t getColumns() const;

        row_index operator[](std::size_t row);
        const_row_index operator[](std::size_t row) const;

        Matrix& operator=(Matrix&& m);
        Matrix& operator=(const std::initializer_list<TDataType>& l);

        template <typename TOtherDataType>
        Matrix& operator=(const Matrix<TOtherDataType>& m);

        static Matrix<TDataType> Transpose(const Matrix<TDataType>& m);

      protected:
        std::size_t rows;
        std::size_t columns;
        std::vector<std::vector<TDataType>> data;
    };

    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const Matrix<TDataType>& rhs);

    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const typename Matrix<TDataType>::each_row& rhs);

    template <typename TDataType>
    Matrix<TDataType> operator+(const typename Matrix<TDataType>::each_row& lhs, const Matrix<TDataType>& rhs);

    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const typename Matrix<TDataType>::each_column& rhs);

    template <typename TDataType>
    Matrix<TDataType> operator+(const typename Matrix<TDataType>::each_column& lhs, const Matrix<TDataType>& rhs);

    template <typename TDataType>
    Matrix<TDataType> operator*(const Matrix<TDataType>& lhs, const Matrix<TDataType>& rhs);

    template <typename TDataType>
    std::ostream& operator<<(std::ostream& os, const Matrix<TDataType>& m);

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::row_index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::row_index::row_index(Matrix<TDataType>& m, std::size_t row)
      : m(m),
        row(row)
    {}

    template <typename TDataType>
    TDataType& Matrix<TDataType>::row_index::operator[](std::size_t column) {
      return m.data[row][column];
    }

    template <typename TDataType>
    Matrix<TDataType>::row_index::operator Matrix<TDataType>() const {
      Matrix<TDataType> result(1, m.columns);

      for (std::size_t column = 0; column < m.columns; ++column) {
        result[1][column] = m[row][column];
      }

      return std::move(result);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::const_row_index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::const_row_index::const_row_index(const Matrix<TDataType>& m, std::size_t row)
      : m(m),
        row(row)
    {}

    template <typename TDataType>
    const TDataType& Matrix<TDataType>::const_row_index::operator[](std::size_t column) const {
      return m.data[row][column];
    }

    template <typename TDataType>
    Matrix<TDataType>::const_row_index::operator Matrix<TDataType>() const {
      Matrix<TDataType> result(1, m.columns);

      for (std::size_t column = 0; column < m.columns; ++column) {
        result[1][column] = m[row][column];
      }

      return std::move(result);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::column_index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::column_index::column_index(Matrix<TDataType>& m, std::size_t column)
      : m(m),
        column(column)
    {}

    template <typename TDataType>
    TDataType& Matrix<TDataType>::column_index::operator[](std::size_t row) {
      return m.data[row][column];
    }

    template <typename TDataType>
    Matrix<TDataType>::column_index::operator Matrix<TDataType>() const {
      Matrix<TDataType> result(m.rows, 1);

      for (std::size_t row = 0; row < m.rows; ++row) {
        result[row][1] = m[row][column];
      }

      return std::move(result);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::const_column_index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::const_column_index::const_column_index(const Matrix<TDataType>& m, std::size_t column)
      : m(m),
        column(column)
    {}

    template <typename TDataType>
    const TDataType& Matrix<TDataType>::const_column_index::operator[](std::size_t row) const {
      return m.data[row][column];
    }

    template <typename TDataType>
    Matrix<TDataType>::const_column_index::operator Matrix<TDataType>() const {
      Matrix<TDataType> result(m.rows, 1);

      for (std::size_t row = 0; row < m.rows; ++row) {
        result[row][1] = m[row][column];
      }

      return std::move(result);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::each_row
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::each_row::each_row(const Matrix<TDataType>& m)
      : m(m)
    {
      std::cout << "copy" << std::endl;
    }

    template <typename TDataType>
    Matrix<TDataType>::each_row::each_row(Matrix<TDataType>&& m)
      : m(m)
    {
      std::cout << "move" << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix::each_column
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::each_column::each_column(const Matrix<TDataType>& m)
      : m(m)
    {}

    template <typename TDataType>
    Matrix<TDataType>::each_column::each_column(Matrix<TDataType>&& m)
      : m(m)
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Matrix
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType>::Matrix(std::size_t rows, std::size_t columns)
      : rows(rows),
        columns(columns)
    {
      assert(rows > 0);
      assert(columns > 0);

      data.resize(rows);
      for(std::vector<TDataType>& row : data) {
        row.resize(columns);
      }
    }

    template <typename TDataType>
    std::size_t Matrix<TDataType>::getRows() const {
      assert(rows == data.capacity());
      return rows;
    }

    template <typename TDataType>
    std::size_t Matrix<TDataType>::getColumns() const {
      assert(columns == data.begin()->capacity());
      return columns;
    }

    template <typename TDataType>
    typename Matrix<TDataType>::row_index Matrix<TDataType>::operator[](std::size_t row) {
      return std::move(row_index(*this, row));
    }

    template <typename TDataType>
    typename Matrix<TDataType>::const_row_index Matrix<TDataType>::operator[](std::size_t row) const {
      return std::move(const_row_index(*this, row));
    }

    template <typename TDataType>
    Matrix<TDataType>& Matrix<TDataType>::operator=(Matrix<TDataType>&& m) {
      assert(rows == m.rows);
      assert(columns == m.columns);

      data = std::move(m.data);
      return *this;
    }

    template <typename TDataType>
    Matrix<TDataType>& Matrix<TDataType>::operator=(const std::initializer_list<TDataType>& l) {
      std::size_t row = 0;
      std::size_t column = 0;

      for (const TDataType& value : l) {
        (*this)[row][column] = value;

        ++column;
        if (column == columns) {
          column = 0;
          ++row;

          if (row == rows) {
            break;
          }
        }
      }

      return *this;
    }

    template <typename TDataType>
    template <typename TOtherDataType>
    Matrix<TDataType>& Matrix<TDataType>::operator=(const Matrix<TOtherDataType>& m) {
      assert(rows == m.rows);
      assert(columns == m.columns);

      for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
          data[row][column] = static_cast<TDataType>(m[row][column]);
        }
      }
    }

    template <typename TDataType>
    Matrix<TDataType> Matrix<TDataType>::Transpose(const Matrix<TDataType>& m) {
      Matrix<TDataType> result(m.columns, m.rows);

      for (std::size_t row = 0; row < m.rows; ++row) {
        for (std::size_t column = 0; column < m.columns; ++column) {
          result[column][row] = m[row][column];
        }
      }

      return std::move(result);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    /// Global operator
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const Matrix<TDataType>& rhs) {
      assert(lhs.rows == rhs.rows);
      assert(lhs.columns == rhs.columns);

      Matrix<TDataType> result(lhs.rows, lhs.columns);

      for (std::size_t row = 0; row < lhs.rows; ++row) {
        for (std::size_t column = 0; column < lhs.columns; ++column) {
          result[row][column] = lhs[row][column] + rhs[row][column];
        }
      }

      return std::move(result);
    }

    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const typename Matrix<TDataType>::each_row& rhs) {
      return std::move(rhs + lhs);
    }

    template <typename TDataType>
    Matrix<TDataType> operator+(const typename Matrix<TDataType>::each_row& lhs, const Matrix<TDataType>& rhs) {
      assert(rhs.rows == 1);
      assert(lhs.m.columns == rhs.columns);

      Matrix<TDataType> result(lhs.m.rows, lhs.m.columns);
      for (std::size_t row = 0; row < lhs.m.rows; ++row) {
        for (std::size_t column = 0; column < lhs.m.columns; ++column) {
          result[row][column] = lhs.m[row][column] + rhs[0][column];
        }
      }

      return std::move(result);
    }

    template <typename TDataType>
    Matrix<TDataType> operator+(const Matrix<TDataType>& lhs, const typename Matrix<TDataType>::each_column& rhs) {
      return std::move(rhs + lhs);
    }

    template <typename TDataType>
    Matrix<TDataType> operator+(const typename Matrix<TDataType>::each_column& lhs, const Matrix<TDataType>& rhs) {
      assert(lhs.m.rows == rhs.rows);
      assert(rhs.columns == 1);

      Matrix<TDataType> result(lhs.m.rows, lhs.m.columns);
      for (std::size_t row = 0; row < lhs.m.rows; ++row) {
        for (std::size_t column = 0; column < lhs.m.columns; ++column) {
          result[row][column] = lhs.m[row][column] + rhs[column][0];
        }
      }

      return std::move(result);
    }

    template <typename TDataType>
    Matrix<TDataType> operator*(const Matrix<TDataType>& lhs, const Matrix<TDataType>& rhs) {
      assert(lhs.columns == rhs.rows);

      Matrix<TDataType> result(lhs.rows, rhs.columns);

      for (std::size_t row = 0; row < lhs.rows; ++row) {
        for (std::size_t column = 0; column < rhs.columns; ++column) {
          TDataType value = TDataType();

          for (std::size_t pos = 0; pos < lhs.columns; ++pos) {
            TDataType lhsv = lhs[row][pos];
            TDataType rhsv = rhs[pos][column];
            value += lhsv * rhsv;
          }

          result[row][column] = value;
        }
      }

      return std::move(result);
    }

    template <typename TDataType>
    std::ostream& operator<<(std::ostream& os, const Matrix<TDataType>& m) {
      for (std::size_t row = 0; row < m.rows; ++row) {
        for (std::size_t column = 0; column < m.columns; ++column) {
          os << std::setw(6) << std::setprecision(2) << std::setfill(' ') << m[row][column];
        }

        os << std::endl;
      }

      return os;
    }
  }
}
