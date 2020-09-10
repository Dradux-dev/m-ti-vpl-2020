#pragma once

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <nmmintrin.h>
#include <type_traits>

namespace  beuth {
  namespace math {
    template <typename T = void>
    struct RegisterInfo {};

    template <>
    struct RegisterInfo<float> {
      using type = __m128;
      using count = std::integral_constant<std::size_t, 4>;
      using hasDiv = std::true_type;

      static inline type setzero() {
        return _mm_setzero_ps();
      }

      static inline type add(type lhs, type rhs) {
        return _mm_add_ps(lhs, rhs);
      }

      static inline type sub(type lhs, type rhs) {
        return _mm_sub_ps(lhs, rhs);
      }

      static inline type mul(type lhs, type rhs) {
        return _mm_mul_ps(lhs, rhs);
      }

      static inline type div(type lhs, type rhs) {
        return _mm_div_ps(lhs, rhs);
      }
    };

    template <>
    struct RegisterInfo<double> {
      using type = __m128d;
      using count = std::integral_constant<std::size_t, 2>;
      using hasDiv = std::true_type;

      static inline type setzero() {
        return _mm_setzero_pd();
      }

      static inline type add(type lhs, type rhs) {
        return _mm_add_pd(lhs, rhs);
      }

      static inline type sub(type lhs, type rhs) {
        return _mm_sub_pd(lhs, rhs);
      }

      static inline type mul(type lhs, type rhs) {
        return _mm_mul_pd(lhs, rhs);
      }

      static inline type div(type lhs, type rhs) {
        return _mm_div_pd(lhs, rhs);
      }
    };

    template <>
    struct RegisterInfo<int> {
      using type = __m128i;
      using count = std::integral_constant<std::size_t, 4>;
      using hasDiv = std::false_type;

      static inline type setzero() {
        return _mm_setzero_si128();
      }

      static inline type add(type lhs, type rhs) {
        return _mm_add_epi32(lhs, rhs);
      }

      static inline type sub(type lhs, type rhs) {
        return _mm_sub_epi32(lhs, rhs);
      }

      static inline type mul(type lhs, type rhs) {
        return _mm_mul_epi32(lhs, rhs);
      }
    };

    template <>
    struct RegisterInfo<std::uint32_t> {
      using type = __m128i;
      using count = std::integral_constant<std::size_t, 4>;
      using hasDiv = std::false_type;

      static inline type setzero() {
        return _mm_setzero_si128();
      }

      static inline type add(type lhs, type rhs) {
        return _mm_add_epi32(lhs, rhs);
      }

      static inline type sub(type lhs, type rhs) {
        return _mm_sub_epi32(lhs, rhs);
      }

      static inline type mul(type lhs, type rhs) {
        return _mm_mul_epi32(lhs, rhs);
      }
    };

    template <typename T>
    class SimdRegister {
      public:

        using Info = RegisterInfo<T>;
        using Register = typename Info::type;
        using Count = typename Info::count;
        using HasDiv = typename Info::hasDiv;

        template <typename U>
        friend SimdRegister<U> operator+(const SimdRegister<U>& lhs, const SimdRegister<U>& rhs);

        inline SimdRegister() {
          data.reg = Info::setzero();
        }

        inline explicit SimdRegister(Register reg) {
          data.reg = reg;
        }

        inline explicit SimdRegister(const std::initializer_list<T>& l) {
          data.reg = Info::setzero();

          for (std::size_t n = 0; n < std::min(Count::value, l.size()); ++n) {
            data.val[n] = *(l.begin()+n);
          }
        }

        SimdRegister& operator=(Register reg) {
          data.reg = reg;
          return *this;
        }

        operator Register() const {
          return data.reg;
        }

        T& operator[](std::size_t index) {
          return data.val[index];
        }

        const T& operator[](std::size_t index) const {
          return data.val[index];
        }

      protected:
        union memory_map {
            Register reg;
            T val[Count::value];
        };

        memory_map data;
    };

    template <typename T>
    SimdRegister<T> operator+(const SimdRegister<T>& lhs, const SimdRegister<T>& rhs) {
      return SimdRegister<T>(SimdRegister<T>::Info::add(lhs, rhs));
    }


    template <typename T>
    SimdRegister<T> operator-(const SimdRegister<T>& lhs, const SimdRegister<T>& rhs) {
      return SimdRegister<T>(SimdRegister<T>::Info::sub(lhs, rhs));
    }

    template <typename T>
    SimdRegister<T> operator*(const SimdRegister<T>& lhs, const SimdRegister<T>& rhs) {
      return SimdRegister<T>(SimdRegister<T>::Info::mul(lhs, rhs));
    }


    template <typename T>
    typename std::enable_if<SimdRegister<T>::HasDiv::value, SimdRegister<T>>::type operator/(const SimdRegister<T>& lhs, const SimdRegister<T>& rhs) {
      return SimdRegister<T>(SimdRegister<T>::Info::div(lhs, rhs));
    }


    template <typename T>
    typename std::enable_if<!SimdRegister<T>::HasDiv::value, SimdRegister<T>>::type operator/(const SimdRegister<T>& lhs, const SimdRegister<T>& rhs) {
      SimdRegister<float> lhsf {
        static_cast<float>(lhs[0]),
        static_cast<float>(lhs[1]),
        static_cast<float>(lhs[2]),
        static_cast<float>(lhs[3])
      };

      SimdRegister<float> rhsf {
        static_cast<float>(rhs[0]),
        static_cast<float>(rhs[1]),
        static_cast<float>(rhs[2]),
        static_cast<float>(rhs[3])
      };

      SimdRegister<float> tmp = lhsf / rhsf;

      return SimdRegister<T> {
        static_cast<T>(tmp[0]),
        static_cast<T>(tmp[1]),
        static_cast<T>(tmp[2]),
        static_cast<T>(tmp[3])
      };
    }


    template <typename T>
    SimdRegister<T> operator+(const SimdRegister<T>& lhs, float rhs) {
      return lhs + SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator-(const SimdRegister<T>& lhs, float rhs) {
      return lhs - SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator*(const SimdRegister<T>& lhs, float rhs) {
      return lhs * SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator/(const SimdRegister<T>& lhs, float rhs) {
      return lhs / SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator+(const SimdRegister<T>& lhs, int rhs) {
      return lhs + SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator-(const SimdRegister<T>& lhs, int rhs) {
      return lhs - SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator*(const SimdRegister<T>& lhs, int rhs) {
      return lhs * SimdRegister<T>{rhs, rhs, rhs, rhs};
    }

    template <typename T>
    SimdRegister<T> operator/(const SimdRegister<T>& lhs, int rhs) {
      return lhs / SimdRegister<T>{rhs, rhs, rhs, rhs};
    }
  }
}
