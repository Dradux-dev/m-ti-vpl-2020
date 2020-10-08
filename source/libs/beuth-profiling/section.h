#pragma once

#include <vector>

#include "beuth-profiling_global.h"
#include "observer.h"
#include "stopwatch.h"

namespace beuth {
  namespace profiling {
    class BEUTHPROFILING_EXPORT Section
    {
      public:
        explicit Section(const std::string& identifier, const std::string& categories, Observer* observer = nullptr);
        virtual ~Section();

        void start();
        void stop();

        virtual void addObserver(Observer* observer);

      protected:
        Stopwatch stopwatch;
        std::vector<Observer*> observers;
        std::string categories;
        std::size_t depth;
    };
  }
}


