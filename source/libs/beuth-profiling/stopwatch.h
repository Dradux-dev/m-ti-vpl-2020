#pragma once

#include <chrono>
#include <string>

#include "beuth-profiling_global.h"

namespace beuth {
  namespace profiling {
    class BEUTHPROFILING_EXPORT Stopwatch {
      public:
        explicit Stopwatch(const std::string& name);
        virtual ~Stopwatch() = default;

        void start();
        void stop();
        void reset();

        std::string getName() const;
        [[nodiscard]] bool isRunning() const;
        [[nodiscard]] bool isStopped() const;

        template <typename T>
        [[nodiscard]] T getDuration() const;

      protected:
        class State {
          public:
            virtual void onEnter(Stopwatch* stopwatch) = 0;
            virtual void onExit(Stopwatch* stopwatch) = 0;
        };

        class Stopped : public State {
            virtual void onEnter(Stopwatch *stopwatch) override;
            virtual void onExit(Stopwatch *stopwatch) override;
        };

        class Running : public State {
            virtual void onEnter(Stopwatch *stopwatch) override;
            virtual void onExit(Stopwatch *stopwatch) override;
        };

        void switchState(State* state);

      protected:
        std::string name;
        State* state = nullptr;
        std::chrono::nanoseconds duration;
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point endTime;

        static State* STOPPED;
        static State* RUNNING;
    };

    template <typename T>
    T Stopwatch::getDuration() const {
      return std::chrono::duration_cast<T>(duration);
    }
  }
}
