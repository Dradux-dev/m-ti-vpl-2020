#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include <profiling.h>

#include "duration.h"
#include "measurement.h"
#include "variant.h"

template <typename TConfig>
class Benchmark
{
  public:
    using variant_t = Variant<TConfig>;
    using variant_ptr_t = std::shared_ptr<variant_t>;
    using variants_t = std::vector<variant_ptr_t>;

    enum class Behavior : std::uint8_t {
      SEQUENTIAL,
      PARALLEL
    };

    struct Format {
        std::size_t width;
        std::size_t precision;
        char fill;

        constexpr inline explicit Format(std::size_t width = 10, std::size_t precision = 4, char fill = ' ')
          : width(width),
            precision(precision),
            fill(fill)
        {}
    };

    inline explicit Benchmark(Behavior behavior = Behavior::SEQUENTIAL)
      : behavior(behavior)
    {}

    template <typename TVariant, typename... TArgs>
    inline variant_ptr_t addVariant(TArgs&&... args);

    void run();

    inline void setFormat(const Format& format);

    inline const Format& getFormat() const;
    inline const variant_t& getVariant(std::size_t column) const;
    inline const variants_t& getVariants() const;

  protected:
    void runSequential();
    void runParallel();

  protected:
    Behavior behavior;
    Format format;
    variants_t variants;
};

template <typename TConfig>
template <typename TVariant, typename... TArgs>
inline typename Benchmark<TConfig>::variant_ptr_t Benchmark<TConfig>::addVariant(TArgs&&... args) {
  BEUTH_PROFILING_FUNCTION("function,beuth,benchmark,allocation");
  variants.emplace_back(new TVariant(std::forward<TArgs>(args)...));
  return *(variants.end()-1);
}

template <typename TConfig>
void Benchmark<TConfig>::run() {
  BEUTH_PROFILING_FUNCTION("function,beuth,benchmark,execution");
  std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

  switch(behavior) {
    case Behavior::SEQUENTIAL:
        runSequential();
    break;
    case Behavior::PARALLEL:
      runParallel();
    break;
  }

  std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
  std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
  std::cout << "Benchmark finished after: " << duration.count() << " seconds" << std::endl;
}

template <typename TConfig>
inline void Benchmark<TConfig>::setFormat(const Format& format) {
  this->format = format;
}

template <typename TConfig>
inline const typename Benchmark<TConfig>::Format& Benchmark<TConfig>::getFormat() const {
  return format;
}

template <typename TConfig>
inline const typename Benchmark<TConfig>::variant_t& Benchmark<TConfig>::getVariant(std::size_t column) const {
  return *variants[column];
}

template <typename TConfig>
inline const typename Benchmark<TConfig>::variants_t& Benchmark<TConfig>::getVariants() const {
  return variants;
}

template <typename TConfig>
void Benchmark<TConfig>::runSequential() {
  BEUTH_PROFILING_FUNCTION("function,beuth,benchmark,execution,sequential");
  for(std::shared_ptr<Variant<TConfig>> variant : variants) {
    variant->setup();
    variant->run();
    variant->cleanup();
  }
}

template <typename TConfig>
void Benchmark<TConfig>::runParallel() {
  BEUTH_PROFILING_FUNCTION("function,beuth,benchmark,execution,parallel");
  for(std::shared_ptr<Variant<TConfig>> variant : variants) {
    variant->setup();
  }

  for(std::shared_ptr<Variant<TConfig>> variant : variants) {
    variant->run();
  }

  for(std::shared_ptr<Variant<TConfig>> variant : variants) {
    variant->cleanup();
  }
}
