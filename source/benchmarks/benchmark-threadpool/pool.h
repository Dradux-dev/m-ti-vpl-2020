#pragma once

#include <variant.hpp>

#include "config.h"

class Pool : public beuth::benchmark::Variant<Config> {
  public:
    Pool(const Config& config);

    virtual void run() override;
};
