#include <iostream>
#include <cmath>
#include "Config.hpp"
#include "Benchmark.hpp"

void Benchmark::Tick(unsigned int status, unsigned int complete) {
  static unsigned int tick = (unsigned int)(complete*_BENCHMARK_DISPLAY_STEP_);
  if((status % tick) == 0 || status == 0 || status == complete) {
    std::cout << "\rProgress: " << (short int)(100.*status/complete) << "%";
    std::cout.flush();
    if(status == complete)
      std::cout << std::endl;
  }
}