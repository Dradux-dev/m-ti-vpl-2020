#include <iostream>

#include <cmath>
#include <stopwatch.h>

beuth::profiling::Stopwatch fn("Total");
beuth::profiling::Stopwatch fn1("Function1()");
beuth::profiling::Stopwatch fn2("Function2()");

void Function1() {
  fn.start();
  fn1.start();

  for (std::size_t n = 0; n < 1000; ++n) {
    std::cout << "Hello World #" << n << std::endl;
  }

  fn1.stop();
  fn.stop();
}

void Function2() {
  fn.start();
  fn2.start();

  for (std::size_t n = 0; n < 1000; ++n) {
    std::cout << "Hello _Sqrt #" << std::sqrt(n) << std::endl;
  }

  fn2.stop();
  fn.stop();
}

int main()
{
  Function1();
  Function2();

  std::cout << std::endl
            << fn1.getName() << ": " << fn1.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl
            << fn2.getName() << ": " << fn2.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl
            << fn.getName() << ": " << fn.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl;

  return 0;
}
