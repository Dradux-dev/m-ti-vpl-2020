#pragma once

#include <variant.hpp>

#include "config.h"

class NoSimd : public beuth::benchmark::Variant<Config>
{
  public:
    NoSimd(const Config& config);

    virtual void run() override;
};

