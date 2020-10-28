#pragma once

#include <variant.hpp>

#include "config.h"

class Async : public beuth::benchmark::Variant<Config> {
  public:
    Async(const Config& config);

    virtual void run() override;
};
