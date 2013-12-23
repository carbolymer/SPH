#include <cmath>
#include "Engine.hpp"
#include "Config.hpp"

Engine::Engine(Fluid* _fluid) : fluid(_fluid) {
  htable = HashTable::GetInstance();
}

void Engine::PerformComputations(unsigned int indexStart, unsigned int indexStop,
      Fluid &computedFluid) {
  htable->Clear();
  for(unsigned int i = 0; i < fluid->x->size(); ++i) {
    htable->Add(i,TVector2(
      fluid->x->at(i),
      fluid->y->at(i)
    ));
  }
  vAdv.clear();
  for(unsigned int i = 0; i < fluid->x->size(); ++i) {
    vAdv.push_back(vRet[i] + _TIME_STEP_*GetAcceleration(i));
    computedFluid.x->at(i) = fluid->x->at(i) + _TIME_STEP_*vAdv[i].X();
    computedFluid.y->at(i) = fluid->y->at(i) + _TIME_STEP_*vAdv[i].Y();
    computedFluid.vx->at(i) = (vRet[i]+vAdv[i]).X();
    computedFluid.vy->at(i) = (vRet[i]+vAdv[i]).Y();
  }

  vRet.clear();
  vRet = std::vector<TVector2>(vAdv);
}

void Engine::CalculateRetardedVelocity() {
  TVector2 acceleration;
  vRet.clear();
  for(unsigned int i = 0; i < fluid->vx->size() ; ++i) {
    acceleration = GetAcceleration(i);
    vRet.push_back(TVector2(
      fluid->vx->at(i) - _TIME_STEP_/2.*acceleration.X(),
      fluid->vy->at(i) - _TIME_STEP_/2.*acceleration.Y()
    ));
  }
}

TVector2 Engine::GetAcceleration(const unsigned int i) {
  TVector2 acceleration(0,0);
  TVector2 kernelGrad(0,0);

  std::vector<unsigned int> indices = htable->FindNN(TVector2(
    fluid->x->at(i),
    fluid->y->at(i)
  ));

  // for(unsigned int j = 0; j < fluid->x->size(); ++j) {
  for(unsigned int hindice = 0, j = 0; hindice < indices.size(); ++hindice) {
    j = indices[hindice];
    kernelGrad = GetSmoothingKernelGrad(i,j);
    if(kernelGrad.X() == 0 && kernelGrad.Y() == 0) {
      continue;
    }
    acceleration += fluid->m->at(j) *
      ( 1/fluid->rho->at(i) + 1/fluid->rho->at(j) + GetViscosity(i,j) ) * kernelGrad;
  }
  acceleration *= -(_NG_C_*_NG_C_/_NG_GAMMA_);
  return acceleration;
}

double Engine::GetViscosity(const unsigned int i, const unsigned j) {
  return 0;
}

double Engine::GetSmoothingKernel(const unsigned int i, const unsigned int j) {
  double x1 = fluid->x->at(i);
  double y1 = fluid->x->at(i);
  double x2 = fluid->x->at(j);
  double y2 = fluid->x->at(j);
  double r = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
  double u = r/_NG_H_;
  double result = 0;
  if(u <= 1) {
    result = 1.-3./2.*u*u+3./4.*u*u*u;
  } else if(u <= 2) {
    result = (2.-u)*(2.-u)*(2.-u)/4.;
  } 
  result *= _NG_SIGMA_/pow(_NG_H_,_NG_NDIM_);
  return result;
}

TVector2 Engine::GetSmoothingKernelGrad(const unsigned int i, const unsigned int j) {
  double x1 = fluid->x->at(i);
  double y1 = fluid->y->at(i);
  double x2 = fluid->x->at(j);
  double y2 = fluid->y->at(j);
  TVector2 r(x1-x2, y1-y2);
  if(r.X() == 0 && r.Y() == 0) {
    return r;
  }
  double rLen = r.Mod();
  TVector2 resultVec(r);
  resultVec /= rLen;
  double u = rLen/_NG_H_;
  double result = 0;
  if(u <= 1) {
    result = -3.*u+9./4.*u*u;
  } else if(u <= 2) {
    result = -3./4.*(2.-u)*(2.-u);
  }
  result *= _NG_SIGMA_/pow(_NG_H_,_NG_NDIM_+1);
  resultVec *= result;
  return resultVec;
}