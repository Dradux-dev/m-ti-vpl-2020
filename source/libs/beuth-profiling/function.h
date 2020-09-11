#pragma once

#include "section.h"

namespace beuth {
  namespace profiling {
    class BEUTHPROFILING_EXPORT Function : protected Section
    {
      public:
        Function(const std::string& identifier, const std::string& categories, Observer* observer = nullptr);
        virtual ~Function();

        virtual void addObserver(Observer *observer) override;
    };
  }
}
