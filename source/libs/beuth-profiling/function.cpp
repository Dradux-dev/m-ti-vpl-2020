#include "function.h"

namespace beuth {
  namespace profiling {
    Function::Function(const std::string& identifier, const std::string& categories, Observer* observer)
      : Section(identifier, categories, observer)
    {
      start();
    }

    void Function::addObserver(Observer *observer) {
      Section::addObserver(observer);
    }

    Function::~Function() {
      stop();
    }
  }
}

