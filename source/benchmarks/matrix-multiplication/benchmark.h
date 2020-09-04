#pragma once

#include <memory>
#include <vector>

#include "measurement.h"
#include "variant.h"

template <typename TConfig>
class Benchmark
{
  public:
    enum class Behavior : std::uint8_t {
      SEQUENTIAL,
      PARALLEL
    };

    inline explicit Benchmark(Behavior behavior = Behavior::SEQUENTIAL)
      : behavior(behavior)
    {}

    template <typename TVariant, typename... TArgs>
    inline std::shared_ptr<Variant<TConfig>> addVariant(TArgs&&... args);

    void run();

  protected:
    void runSequential();
    void runParallel();

  protected:
    Behavior behavior;
    std::vector<std::shared_ptr<Variant<TConfig>>> variants;
};

template <typename TConfig>
template <typename TVariant, typename... TArgs>
inline std::shared_ptr<Variant<TConfig>> Benchmark<TConfig>::addVariant(TArgs&&... args) {
  variants.emplace_back(new TVariant(std::forward<TArgs>(args)...));
  return *(variants.end()-1);
}

template <typename TConfig>
void Benchmark<TConfig>::run() {
  switch(behavior) {
    case Behavior::SEQUENTIAL:
        runSequential();
    break;
    case Behavior::PARALLEL:
      runParallel();
    break;
  }
}

template <typename TConfig>
void Benchmark<TConfig>::runSequential() {
  for(std::shared_ptr<Variant<TConfig>> variant : variants) {
    variant->setup();
    variant->run();
    variant->cleanup();
  }
}

template <typename TConfig>
void Benchmark<TConfig>::runParallel() {
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

