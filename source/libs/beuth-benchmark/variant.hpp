#pragma once

#include <string>
#include <vector>

#include "measurement.h"

namespace beuth {
  namespace benchmark {
    template <typename TConfig>
    class Variant
    {
      public:
        inline explicit Variant(const TConfig& config, const std::string& name = "")
          : config(config),
            name(name)
        {}

        Variant(const Variant&) = default;
        Variant(Variant&&) = default;

        virtual void setup();
        virtual void run();
        virtual void cleanup();

        const std::string& getName() const;
        const Measurement& getMeasurement(std::size_t index) const;
        std::size_t getRows() const;

      protected:
        const TConfig& config;
        std::string name;
        std::vector<Measurement> measurements;
    };

    template <typename TConfig>
    void Variant<TConfig>::setup() {
    }

    template <typename TConfig>
    void Variant<TConfig>::run() {
    }

    template <typename TConfig>
    void Variant<TConfig>::cleanup() {
    }

    template <typename TConfig>
    const std::string& Variant<TConfig>::getName() const {
      return name;
    }

    template <typename TConfig>
    const Measurement& Variant<TConfig>::getMeasurement(std::size_t index) const {
      return measurements[index];
    }

    template <typename TConfig>
    std::size_t Variant<TConfig>::getRows() const {
      return measurements.size();
    }
  }
}
