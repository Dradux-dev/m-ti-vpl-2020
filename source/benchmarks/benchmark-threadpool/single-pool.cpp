#include "single-pool.h"

#include <batch.hpp>
#include <measurement.h>
#include <threadpool.h>

SinglePool::SinglePool(const Config& config)
  : beuth::benchmark::Variant<Config>(config, "SinglePool")
{}

void SinglePool::run() {
  beuth::thread::Threadpool pool(20);

  for(std::size_t amount = config.tasks.min; amount <= config.tasks.max; ++amount) {
    std::cout << "Running SinglePool " << amount << std::endl;
    beuth::benchmark::Measurement measurement;

    for(std::size_t iteration = 0; iteration < config.iterations; ++iteration) {
      measurement.start();

      beuth::thread::Batch batch(pool, amount);

      for(std::size_t n = 0; n < amount; ++n) {
        batch.process(config.task);
      }

      batch.wait();

      measurement.stop();
    }

    measurements.push_back(std::move(measurement));
  }
}
