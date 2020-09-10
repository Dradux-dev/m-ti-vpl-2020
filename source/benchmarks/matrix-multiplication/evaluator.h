#pragma once

#include <limits>

#include "benchmark.h"
#include "duration.h"

template <typename TConfig>
class Evaluator {
  public:
    using ValueReader = std::function<double()>;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Evaluator<U>& evaluator);

    inline explicit Evaluator(const Benchmark<TConfig>& benchmark, Duration durationCast = Duration::MILLISECONDS)
      : benchmark(benchmark),
        durationCast(durationCast)
    {}

    virtual ~Evaluator() = default;

    std::size_t getColumns() const;
    std::size_t getRows() const;
    virtual double getValue(std::size_t column, std::size_t row) const = 0;
    std::string getTitle(std::size_t column) const;

  protected:
    const Benchmark<TConfig>& benchmark;
    Duration durationCast;
};

template <typename TConfig>
class MinEvaluator : public Evaluator<TConfig> {
  public:
    inline explicit MinEvaluator(const Benchmark<TConfig>& benchmark, Duration durationCast = Duration::MILLISECONDS)
      : Evaluator<TConfig>(benchmark, durationCast)
    {}

    virtual double getValue(std::size_t column, std::size_t row) const override;
};

template <typename TConfig>
class MaxEvaluator : public Evaluator<TConfig> {
  public:
    inline explicit MaxEvaluator(const Benchmark<TConfig>& benchmark, Duration durationCast = Duration::MILLISECONDS)
      : Evaluator<TConfig>(benchmark, durationCast)
    {}

    virtual double getValue(std::size_t column, std::size_t row) const override;
};

template <typename TConfig>
class AvgEvaluator : public Evaluator<TConfig> {
  public:
    inline explicit AvgEvaluator(const Benchmark<TConfig>& benchmark, Duration durationCast = Duration::MILLISECONDS)
      : Evaluator<TConfig>(benchmark, durationCast)
    {}

    virtual double getValue(std::size_t column, std::size_t row) const override;
};

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const Evaluator<TConfig>& evaluator);

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const MinEvaluator<TConfig>& evaluator);

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const MaxEvaluator<TConfig>& evaluator);

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const AvgEvaluator<TConfig>& evaluator);

template <typename TConfig>
std::size_t Evaluator<TConfig>::getColumns() const {
  return benchmark.getVariants().size();
}

template <typename TConfig>
std::size_t Evaluator<TConfig>::getRows() const {
  if (getColumns() == 0) {
    // There is no row, if there is not at least one column
    return 0;
  }

  std::size_t minRows = std::numeric_limits<std::size_t>::max();
  for (const typename Benchmark<TConfig>::variant_ptr_t variant : benchmark.getVariants()) {
    std::size_t rows = variant->getRows();
    if (rows < minRows) {
      minRows = rows;
    }
  }

  return minRows;
}

template <typename TConfig>
std::string Evaluator<TConfig>::getTitle(std::size_t column) const {
  return benchmark.getVariant(column).getName();
}

template <typename TConfig>
double MinEvaluator<TConfig>::getValue(std::size_t column, std::size_t row) const {
  return MeasurementConverter(Evaluator<TConfig>::benchmark.getVariant(column).getMeasurement(row), static_cast<std::size_t>(Evaluator<TConfig>::durationCast)).min();
}

template <typename TConfig>
double MaxEvaluator<TConfig>::getValue(std::size_t column, std::size_t row) const {
  return MeasurementConverter(Evaluator<TConfig>::benchmark.getVariant(column).getMeasurement(row), static_cast<std::size_t>(Evaluator<TConfig>::durationCast)).max();
}

template <typename TConfig>
double AvgEvaluator<TConfig>::getValue(std::size_t column, std::size_t row) const {
  return MeasurementConverter(Evaluator<TConfig>::benchmark.getVariant(column).getMeasurement(row), static_cast<std::size_t>(Evaluator<TConfig>::durationCast)).avg();
}

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const Evaluator<TConfig>& evaluator) {
  const std::size_t columns = evaluator.getColumns();
  const std::size_t rows = evaluator.getRows();
  const typename Benchmark<TConfig>::Format& format = evaluator.benchmark.getFormat();

  os << "#" << std::setfill(' ') << std::setw(format.width-1) << "Test";
  for (std::size_t column = 0; column < columns; ++column) {
    std::string title = evaluator.getTitle(column);
    os << std::setfill(' ') << std::setw(format.width) << title.substr(0, std::min(format.width, title.length())) << " ";
  }
  os << std::endl;


  for (std::size_t row = 0; row < rows; ++row) {
    os << std::setfill(format.fill) << std::setw(format.width) << (row+1) << " ";

    for (std::size_t column = 0; column < columns; ++column) {
      os << std::setfill(format.fill) << std::setw(format.width) << std::setprecision(format.precision) << evaluator.getValue(column, row) << " ";
    }
    os << std::endl;
  }

  return os;
}

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const MinEvaluator<TConfig>& evaluator) {
  return os << (const Evaluator<TConfig>&)evaluator;
}

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const MaxEvaluator<TConfig>& evaluator) {
  return os << (const Evaluator<TConfig>&)evaluator;
}

template <typename TConfig>
std::ostream& operator<<(std::ostream& os, const AvgEvaluator<TConfig>& evaluator) {
  return os << (const Evaluator<TConfig>&)evaluator;
}
