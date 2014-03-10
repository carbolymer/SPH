#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <vector>
#include "TMath.h"
#include "TVector2.h"
#include "Fluid.hpp"
#include "HashTable.hpp"

class Engine {
private:
  double const alpha;
  HashTable *htable;
  Fluid * const fluid;
  std::vector<TVector2> vAdv; // advanced velocity by tau/2
  std::vector<TVector2> vRet; // retarded velocity by tau/2
  Double_t meanTotalEnergy = 0;
public:
  Engine(Fluid* _fluid);
  ~Engine() {};
  void CalculateRetardedVelocity();
  void CalculateAdvancedVelocity();
  void PerformComputations(const unsigned int indexStart, unsigned int indexStop,
      Fluid &computedFluid);
  TVector2 GetAcceleration(const unsigned int);
  Double_t GetPotential(const unsigned int);
  double GetMassDensity(const unsigned int);
  double GetViscosity(const unsigned int, const unsigned int);
  double GetSmoothingKernel(const unsigned int, const unsigned int);
  TVector2 GetSmoothingKernelGrad(const unsigned int, const unsigned int);
  double GetSmoothingKernelLapl(const unsigned int, const unsigned int);
  TVector2 GetSurfaceNormal(const unsigned int);
  TVector2 GetArtificialForce(const unsigned int, const unsigned int);
  Double_t GetArtificialPotential(const unsigned int, const unsigned int);
  const Double_t GetMeanTotalEnergy() const { return meanTotalEnergy; };
};

#endif