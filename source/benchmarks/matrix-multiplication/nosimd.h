#pragma once

#include "config.h"
#include "variant.h"

class NoSimd : public Variant<Config>
{
  public:
    NoSimd(const Config& config);

    virtual void run() override;
};

