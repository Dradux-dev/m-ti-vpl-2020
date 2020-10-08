#pragma once

#include <chrono>
#include <fstream>

#include "beuth-profiling_global.h"
#include "exporter.h"

namespace beuth {
  namespace profiling {
    class BEUTHPROFILING_EXPORT Chrome : public Exporter
    {
      public:
        Chrome();
        virtual ~Chrome();

        virtual void beginSession(const std::string &name) override;
        virtual void endSession() override;
        virtual void addEntry(const std::string &name, const std::string& categories, std::size_t depth, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) override;

      protected:
        std::ofstream file;
        volatile bool firstEntry;
    };
  }
}
