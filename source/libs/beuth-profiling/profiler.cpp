#include "profiler.h"

namespace beuth {
  namespace profiling {
    Profiler& Profiler::Instance() {
      static Profiler profiler;
      return profiler;
    }

    void Profiler::sectionFinished(const std::string &identifier, const std::string& categories, std::size_t depth, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) {
      for (Exporter* exporter : exporters) {
        if (exporter) {
          exporter->addEntry(identifier, categories, depth, startTime, duration);
        }
      }
    }

    void Profiler::beginSession(const std::string &name) {
      for (Exporter* exporter : exporters) {
        if (exporter) {
          exporter->beginSession(name);
        }
      }
    }

    void Profiler::endSession() {
      for (Exporter* exporter : exporters) {
        if (exporter) {
          exporter->endSession();
        }
      }
    }
  }
}
