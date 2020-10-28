#pragma once

#include <variant.hpp>

#include "config.h"

class SinglePool : public beuth::benchmark::Variant<Config> {
  public:
    SinglePool(const Config& config);

    virtual void run() override;
};
