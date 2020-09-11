#include <iostream>

#include <cmath>
#include <thread>

#include <profiling.h>

beuth::profiling::Stopwatch fn("Total");
beuth::profiling::Stopwatch fn1("Function1()");
beuth::profiling::Stopwatch fn2("Function2()");

void Function1() {
  BEUTH_PROFILING_FUNCTION("function,test,helloWorld");
  fn.start();
  fn1.start();

  for (std::size_t n = 0; n < 1000; ++n) {
    std::cout << "Hello World #" << n << std::endl;
  }

  fn1.stop();
  fn.stop();
}

void Function2() {
  BEUTH_PROFILING_FUNCTION("function,test,math");
  fn.start();
  fn2.start();

  for (std::size_t n = 0; n < 1000; ++n) {
    std::cout << "Hello _Sqrt #" << std::sqrt(n) << std::endl;
  }

  fn2.stop();
  fn.stop();
}

void RunBenchmarks() {
  BEUTH_PROFILING_FUNCTION("function,test,sequence");
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  Function1();
  std::this_thread::sleep_for(std::chrono::milliseconds(7));
  Function2();
  std::this_thread::sleep_for(std::chrono::milliseconds(12));
}

int main()
{
  BEUTH_PROFILING_CHROME_EXPORT();
  BEUTH_PROFILING_BEGIN_SESSION("profiling");
  BEUTH_PROFILING_BEGIN_SECTION(Calc, "section,main");
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  RunBenchmarks();
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
  BEUTH_PROFILING_END_SECTION(Calc);

  BEUTH_PROFILING_BEGIN_SECTION(Result, "section,main");
  std::cout << std::endl
            << fn1.getName() << ": " << fn1.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl
            << fn2.getName() << ": " << fn2.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl
            << fn.getName() << ": " << fn.getDuration<std::chrono::milliseconds>().count() << "ms" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  BEUTH_PROFILING_END_SECTION(Result);

  BEUTH_PROFILING_END_SESSION();
  return 0;
}
