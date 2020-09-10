#pragma once

#include <assert.h>
#include <cstdint>

struct Config {
    std::size_t minSize;
    std::size_t maxSize;
    std::size_t stepSize = 1;
    std::size_t iterations;

    constexpr inline Config(std::size_t minSize, std::size_t maxSize, std::size_t stepSize, std::size_t iterations)
      : minSize(minSize),
        maxSize(maxSize),
        stepSize(stepSize),
        iterations(iterations)
    {}

    constexpr inline Config(std::size_t minSize, std::size_t maxSize, std::size_t iterations)
      : minSize(minSize),
        maxSize(maxSize),
        iterations(iterations)
    {}
};
