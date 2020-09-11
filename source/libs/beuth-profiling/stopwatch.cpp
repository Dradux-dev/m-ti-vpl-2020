#include "stopwatch.h"

namespace beuth {
  namespace profiling {
    Stopwatch::State* Stopwatch::STOPPED = new Stopwatch::Stopped;
    Stopwatch::State* Stopwatch::RUNNING = new Stopwatch::Running;

    void Stopwatch::Stopped::onEnter(Stopwatch*) {
    }

    void Stopwatch::Stopped::onExit(Stopwatch*) {
    }

    void Stopwatch::Running::onEnter(Stopwatch *stopwatch) {
      stopwatch->startTime = std::chrono::high_resolution_clock::now();
    }

    void Stopwatch::Running::onExit(Stopwatch *stopwatch) {
      std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
      stopwatch->duration += std::chrono::duration_cast<std::chrono::microseconds>(endTime - stopwatch->startTime);
    }

    Stopwatch::Stopwatch(const std::string& name)
      : name(name),
        duration()
    {}

    void Stopwatch::start() {
      switchState(Stopwatch::RUNNING);
    }

    void Stopwatch::stop() {
      switchState(Stopwatch::STOPPED);
    }

    void Stopwatch::reset() {
      switchState(Stopwatch::STOPPED);
      duration = std::chrono::microseconds();
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

    std::chrono::high_resolution_clock::time_point Stopwatch::getStartTime() const {
      return startTime;
    }
  }
}

