#pragma once

#include <cstdint>
#include <functional>

struct Config {
    struct {
        std::size_t min;
        std::size_t max;
    } tasks;
    std::function<void()> task;
    std::size_t iterations;
};
