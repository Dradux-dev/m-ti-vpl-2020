#pragma once

#include "config.h"
#include "variant.h"

class Simd : public Variant<Config>
{
  public:
    Simd(const Config& config);

    virtual void run() override;
};

