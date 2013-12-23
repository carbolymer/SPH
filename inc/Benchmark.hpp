#ifndef _BENCHMARK_HPP_
#define _BENCHMARK_HPP_

class Benchmark {
private:
  Benchmark();
public:
  static void Tick(unsigned int status, unsigned int complete);
};

#endif
