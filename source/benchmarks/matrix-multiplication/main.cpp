#include <iostream>
#include <chrono>
#include <limits>

#include <matrix.hpp>

using namespace std;

struct Measurement {
    using time_point = std::chrono::high_resolution_clock::time_point;
    using duration = std::chrono::nanoseconds;

    duration min;
    duration max;
    duration avg;
    std::size_t count;
    time_point startTime;

    inline Measurement()
      : min(duration::max()),
        max(duration::min()),
        avg(duration::zero()),
        count(0)
    {}

    Measurement(const Measurement&) = default;
    Measurement(Measurement&&) = default;

    inline void start() {
      startTime = now();
      ++count;
    }

    void stop() {
      time_point stopTime = now();
      duration elapsed = std::chrono::duration_cast<duration>(stopTime - startTime);

      min = std::min(min, elapsed);
      max = std::max(max, elapsed);

      double weight = 1 / count;
      avg = duration(static_cast<duration::rep>((1 - weight) * avg.count() + weight * elapsed.count()));
    }

  protected:
    inline time_point now() {
      return std::chrono::high_resolution_clock::now();
    }
};

struct MeasurementConverter {
    const Measurement& measurement;
    using duration = Measurement::duration;
    std::size_t denominator;

    inline MeasurementConverter(const Measurement& measurement, std::size_t denominator)
      : measurement(measurement),
        denominator(denominator)
    {}

    inline double min() const {
      return convert(measurement.min);
    }

    inline double max() const {
      return convert(measurement.max);
    }

    inline double avg() const {
      return convert(measurement.avg);
    }

  protected:
    inline double factor() const {
      return static_cast<double>(denominator) / duration::period::den;
    }

    inline double convert(const duration& d) const {
      return std::chrono::duration_cast<std::chrono::duration<double, duration::period>>(d).count() * factor();
    }
};

std::ostream& operator<<(std::ostream& os, const MeasurementConverter& m) {
  return os << "[Min: " << m.min()
            << ", Max: " << m.max()
            << ", Avg: " << m.avg()
            << "]";
}

struct RandomNumberGenerator {
    static int Generate(int min, int max) {
      return (static_cast<int>(Instance()) % max) + min;
    }

    static double Generate(double min, double max) {
      return (static_cast<double>(Instance()) * max) + min;
    }

    operator int() const {
      return rand();
    }

    operator double() const {
      return static_cast<double>(rand()) / static_cast<double>(std::numeric_limits<int>::max());
    }

  protected:
    int generate() {
      return rand();
    }

  private:
    static RandomNumberGenerator& Instance() {
      static RandomNumberGenerator rng;
      return rng;
    }

    inline RandomNumberGenerator() {
      srand(time(NULL));
    }
};

template <typename T>
struct MatrixRandomizer {
    void operator ()(beuth::math::Matrix<T>& m) {
      for (std::size_t row = 0; row < m.getRows(); ++row) {
        for (std::size_t col = 0; col < m.getColumns(); ++col) {
          m[row][col] = RandomNumberGenerator::Generate(0.0, 1.0);
        }
      }
    }
};

int main(int, char**)
{
  using Matrix = beuth::math::Matrix<float>;
  using MatrixRandomizer = MatrixRandomizer<float>;

  // Setup range
  constexpr const std::size_t MIN_SIZE = 1;
  constexpr const std::size_t MAX_SIZE = 100;
  constexpr const std::size_t TEST_COUNT = 100;

  // Check setup
  static_assert (MIN_SIZE > 0, "Matrix 0x0 is not allowed (min size)");
  static_assert (MAX_SIZE >= MIN_SIZE, "Max has to be greater or equal to min");
  static_assert (TEST_COUNT > 0, "Can't perform less than 1 test");

  MatrixRandomizer randomizeMatrix;
  std::vector<Measurement> measurements;


  for (std::size_t size = MIN_SIZE; size <= MAX_SIZE; ++size) {
    std::cout << "Performing benchmark for " << size << "x" << size << " matrix";
    std::cout.flush();

    Measurement measure;

    for (std::size_t test = 1; test <= TEST_COUNT; ++test) {
      if ((test - 1) % (TEST_COUNT / 20) == 0) {
        std::cout << "." << std::flush;
      }

      Matrix a = Matrix(size, size);
      Matrix b = Matrix(size, size);
      randomizeMatrix(a);
      randomizeMatrix(b);

      measure.start();
      Matrix r = a * b;
      measure.stop();
    }

    measurements.push_back(std::move(measure));
    std::cout << MeasurementConverter(measure, 1000) << std::endl;
  }

  return 0;
}
