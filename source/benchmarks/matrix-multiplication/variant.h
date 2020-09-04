#pragma once

#include <vector>

#include "measurement.h"

template <typename TConfig>
class Variant
{
  public:
    inline explicit Variant(const TConfig& config)
      : config(config)
    {}

    Variant(const Variant&) = default;
    Variant(Variant&&) = default;

    virtual void setup();
    virtual void run();
    virtual void cleanup();

  protected:
    const TConfig& config;
    std::vector<Measurement> measurements;
};

template <typename TConfig>
void Variant<TConfig>::setup() {
}

template <typename TConfig>
void Variant<TConfig>::run() {
}

template <typename TConfig>
void Variant<TConfig>::cleanup() {
}
