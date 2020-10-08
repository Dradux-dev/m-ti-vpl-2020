#include "chrome.h"

#include <iomanip>
#include <iostream>
#include <thread>

namespace beuth {
  namespace profiling {
    Chrome::Chrome() {}

    void Chrome::beginSession(const std::string &name) {
      if (file.is_open()) {
        std::cerr << "Automatically ended old session" << std::endl;
        endSession();
      }

      file.open(name + ".json");
      if (!file.is_open()) {
        std::cerr << "Can open session file \"" << name << ".json\"" << std::endl;
        return;
      }

      file << "{ \"otherData\" : { \"version\" : \"Beuth Profiling 1.0\" }, \"traceEvents\" : [";
      firstEntry = true;
    }

    void Chrome::addEntry(const std::string &name, const std::string& categories, std::size_t depth, std::chrono::high_resolution_clock::time_point startTime, std::chrono::microseconds duration) {
      if (!file.is_open()) {
        std::cerr << "Can not add entry, because no session was started" << std::endl;
      }

      long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();

      // Uses pid as thread id
      // Uses tid as layer id
      file << (firstEntry ? "" : ", ") << "{ "
           << "\"name\" : \"" << name << "\", "
           << "\"cat\" : \"" << categories << "\", "
           << "\"ph\" : \"X\", "
           << "\"ts\" : " << start << ", "
           << "\"dur\" : " << duration.count() << ", "
           << "\"pid\" : " << std::this_thread::get_id() << ", "
           << "\"tid\" : " << depth << " "
           << "}"
           << std::flush;

      firstEntry = false;
    }

    void Chrome::endSession() {
      if (!file.is_open()) {
        std::cerr << "Can not end session, because it never started" << std::endl;
        return;
      }

      file << "] }" << std::flush;
      file.close();
    }

    Chrome::~Chrome() {
      if (file.is_open()) {
        endSession();
      }
    }
  }
}
