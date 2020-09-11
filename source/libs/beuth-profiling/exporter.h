#pragma once

#include <chrono>
#include <string>

namespace beuth {
  namespace profiling {
    class Exporter
    {
      public:
        virtual void beginSession(const std::string& name) = 0;
        virtual void endSession() = 0;
        virtual void addEntry(const std::string& name, const std::string& categories, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) = 0;
    };
  }
}

