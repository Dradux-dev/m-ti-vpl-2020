#include "async.h"

#include <future>

#include <measurement.h>

Async::Async(const Config& config)
  : beuth::benchmark::Variant<Config>(config, "Async")
{}

void Async::run() {
  for(std::size_t amount = config.tasks.min; amount <= config.tasks.max; ++amount) {
    std::cout << "Running Async " << amount << std::endl;
    beuth::benchmark::Measurement measurement;

    for(std::size_t iteration = 0; iteration < config.iterations; ++iteration) {
      measurement.start();

      std::vector<std::future<void>> futures;
      futures.reserve(amount);

      for(std::size_t n = 0; n < amount; ++n) {
        futures.emplace_back(std::async(config.task));
      }

      for(std::future<void>& f : futures) {
        f.wait();
      }

      measurement.stop();
    }

    measurements.push_back(std::move(measurement));
  }
}
