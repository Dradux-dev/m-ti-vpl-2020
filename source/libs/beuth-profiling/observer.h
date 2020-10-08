#pragma once

#include <chrono>
#include <string>

namespace beuth {
  namespace profiling {
    class Observer
    {
      public:
        virtual void sectionFinished(const std::string& identifier, const std::string& categories, std::size_t depth, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) = 0;
    };
  }
}
