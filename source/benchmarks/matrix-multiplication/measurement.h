#pragma once

#include <chrono>
#include <iostream>

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

    inline void reset() {
      min = duration::max();
      max = duration::min();
      avg = duration::zero();
      count = 0;
    }

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

inline std::ostream& operator<<(std::ostream& os, const MeasurementConverter& m) {
  return os << "[Min: " << m.min()
            << ", Max: " << m.max()
            << ", Avg: " << m.avg()
            << "]";
}
