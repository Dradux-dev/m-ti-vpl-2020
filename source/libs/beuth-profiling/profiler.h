#pragma once

#include <vector>

#include "beuth-profiling_global.h"
#include "exporter.h"
#include "observer.h"

namespace beuth {
  namespace profiling {
    class BEUTHPROFILING_EXPORT Profiler : public Observer
    {
      public:
        static Profiler& Instance();

        virtual void sectionFinished(const std::string &identifier, const std::string& categories, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) override;

        void beginSession(const std::string& name);
        void endSession();

        template <typename T, typename... TArgs>
        void addExporter(TArgs... args);

      private:
        Profiler() = default;
        Profiler(const Profiler&) = default;
        Profiler& operator=(const Profiler&) = default;

      protected:
        std::vector<Exporter*> exporters;
    };

    template <typename T, typename... TArgs>
    void Profiler::addExporter(TArgs... args) {
      exporters.push_back(new T(std::forward(args)...));
    }
  }
}

