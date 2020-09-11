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
        [[nodiscard]] typename std::enable_if<std::is_same<T, std::chrono::microseconds>::value, T>::type getDuration() const;

        template <typename T>
        [[nodiscard]] typename std::enable_if<!std::is_same<T, std::chrono::microseconds>::value, T>::type getDuration() const;

        std::chrono::high_resolution_clock::time_point getStartTime() const;

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
        std::chrono::microseconds duration;
        std::chrono::high_resolution_clock::time_point startTime;

        static State* STOPPED;
        static State* RUNNING;
    };

    template <typename T>
    [[nodiscard]] typename std::enable_if<std::is_same<T, std::chrono::microseconds>::value, T>::type Stopwatch::getDuration() const {
      return duration;
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<!std::is_same<T, std::chrono::microseconds>::value, T>::type Stopwatch::getDuration() const {
      return std::chrono::duration_cast<T>(duration);
    }
  }
}
