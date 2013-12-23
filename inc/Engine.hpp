#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <vector>
#include <Fluid.hpp>

class Engine {
private:
  Engine(){};
  static std::vector<double> vAdv; // advanced velocity by tau/2
  static std::vector<double> vRet; // retarded velocity by tau/2
public:
  static void CalculateRetardedVelocity(const Fluid&);
  static void CalculateAdvancedVelocity(const Fluid&);
  static void PerformComputations(unsigned int indexStart, unsigned int indexStop,
    const Fluid &fluid, Fluid &computedFluid);
};

#endif