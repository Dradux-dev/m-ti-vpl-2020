#pragma once

#include <variant.hpp>

#include "config.h"

class Simd : public beuth::benchmark::Variant<Config>
{
  public:
    Simd(const Config& config);

    virtual void run() override;
};

