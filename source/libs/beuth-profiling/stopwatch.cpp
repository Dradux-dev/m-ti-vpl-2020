#include "stopwatch.h"

namespace beuth {
  namespace profiling {
    Stopwatch::State* Stopwatch::STOPPED = new Stopwatch::Stopped;
    Stopwatch::State* Stopwatch::RUNNING = new Stopwatch::Running;

    void Stopwatch::Stopped::onEnter(Stopwatch *stopwatch) {
      stopwatch->startTime = std::chrono::high_resolution_clock::time_point();
      stopwatch->endTime = std::chrono::high_resolution_clock::time_point();
    }

    void Stopwatch::Stopped::onExit(Stopwatch*) {
    }

    void Stopwatch::Running::onEnter(Stopwatch *stopwatch) {
      stopwatch->startTime = std::chrono::high_resolution_clock::now();
    }

    void Stopwatch::Running::onExit(Stopwatch *stopwatch) {
      stopwatch->endTime = std::chrono::high_resolution_clock::now();
      stopwatch->duration += std::chrono::duration_cast<std::chrono::nanoseconds>(stopwatch->endTime - stopwatch->startTime);
    }

    Stopwatch::Stopwatch(const std::string& name)
      : name(name)
    {}

    void Stopwatch::start() {
      switchState(Stopwatch::RUNNING);
    }

    void Stopwatch::stop() {
      switchState(Stopwatch::STOPPED);
    }

    void Stopwatch::reset() {
      switchState(Stopwatch::STOPPED);
      duration = std::chrono::nanoseconds();
    }

    void Stopwatch::switchState(State *newState) {
      if (state) {
        state->onExit(this);
      }

      state = newState;

      if (newState) {
        newState->onEnter(this);
      }
    }

    std::string Stopwatch::getName() const {
      return name;
    }

    bool Stopwatch::isRunning() const {
      return (state == Stopwatch::RUNNING);
    }

    bool Stopwatch::isStopped() const {
      return (state == Stopwatch::STOPPED);
    }
  }
}

