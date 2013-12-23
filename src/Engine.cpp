#include "Engine.hpp"
#include "Config.hpp"

void Engine::PerformComputations(unsigned int indexStart, unsigned int indexStop, const Fluid &fluid,
      Fluid &computedFluid) {
  for(unsigned int j = indexStart; j < indexStop; ++j) {
    computedFluid.x->at(j) = fluid.x->at(j) + 50./_NUMBER_OF_STEPS_;
    computedFluid.y->at(j) = fluid.y->at(j) - 50./_NUMBER_OF_STEPS_;
  }
}

void Engine::CalculateRetardedVelocity(const Fluid &fluid) {
  ;
}

void Engine::CalculateAdvancedVelocity(const Fluid &fluid) {
  ;
}

