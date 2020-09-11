#include "section.h"

namespace beuth {
  namespace profiling {
    Section::Section(const std::string& identifier, const std::string& categories, Observer* observer)
      : stopwatch(identifier),
        categories(categories)
    {
      addObserver(observer);
    }

    void Section::start() {
      stopwatch.start();
    }

    void Section::stop() {
      stopwatch.stop();

      for (Observer* observer : observers) {
        if (observer) {
          observer->sectionFinished(stopwatch.getName(), categories, stopwatch.getStartTime(), stopwatch.getDuration<std::chrono::microseconds>());
        }
      }
    }

    void Section::addObserver(Observer *observer) {
      observers.push_back(observer);
    }
  }
}
