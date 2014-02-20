#include <cmath>
#include "Engine.hpp"
#include "Config.hpp"

Engine::Engine(Fluid* _fluid) : alpha(37./32/M_PI/pow(_NG_H_,7)), fluid(_fluid)  {
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
    computedFluid.rho->at(i) = GetMassDensity(i);
  }

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

double Engine::GetMassDensity(const unsigned int i) {
  double density = 0;

  std::vector<unsigned int> indices = htable->FindNN(TVector2(
    fluid->x->at(i),
    fluid->y->at(i)
  ));

  for(unsigned int hindice = 0, j = 0; hindice < indices.size(); ++hindice) {
    j = indices[hindice];
    density += fluid->m->at(j)*GetSmoothingKernel(i,j);
  }
  return density;
}

TVector2 Engine::GetAcceleration(const unsigned int i) {
  TVector2 acceleration(0,0);
  TVector2 kernelGrad(0,0);

  std::vector<unsigned int> indices = htable->FindNN(TVector2(
    fluid->x->at(i),
    fluid->y->at(i)
  ));

  TVector2 surfaceNormal = _NG_SIGMA_*GetSurfaceNormal(i)/fluid->rho->at(i);
  // const double internalPressureConst = -(_NG_C_*_NG_C_/_NG_GAMMA_);

  for(unsigned int hindice = 0, j = 0; hindice < indices.size(); ++hindice) {
    j = indices[hindice];
    kernelGrad = GetSmoothingKernelGrad(i,j);
    if(kernelGrad.X() == 0 && kernelGrad.Y() == 0) {
      continue;
    }
    // - pressure
    // acceleration += internalPressureConst*fluid->m->at(j) *
    //   ( 1/fluid->rho->at(i) + 1/fluid->rho->at(j) + GetViscosity(i,j) ) * kernelGrad;
    // - surface tension
    // acceleration -= surfaceNormal*fluid->m->at(j)/fluid->rho->at(j)*GetSmoothingKernelLapl(i,j);
    acceleration += GetArtificialForce(i,j);
  }
  acceleration /= fluid->m->at(i);
  return acceleration;
}

double Engine::GetViscosity(const unsigned int i, const unsigned j) {
  return 0;
}

double Engine::GetSmoothingKernel(const unsigned int i, const unsigned int j) {
  double const x1 = fluid->x->at(i);
  double const y1 = fluid->x->at(i);
  double const x2 = fluid->x->at(j);
  double const y2 = fluid->x->at(j);
  double const rsq = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
  double const hsq = _NG_H_*_NG_H_;
  double result = 0;
  if(rsq <= hsq) {
    result = (hsq-rsq)*(hsq-rsq)*(hsq-rsq)*alpha;
  }
  return result;
}

TVector2 Engine::GetSmoothingKernelGrad(const unsigned int i, const unsigned int j) {
  double const x1 = fluid->x->at(i);
  double const y1 = fluid->y->at(i);
  double const x2 = fluid->x->at(j);
  double const y2 = fluid->y->at(j);
  if(x1 == x2 && y1 == y2)
    return TVector2(0,0);
  TVector2 r(x2-x1, y2-y1);
  double const rsq = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
  double const hsq = _NG_H_*_NG_H_;
  if(rsq <= hsq)
    r *= 3./2.*alpha*(hsq-rsq)*(hsq-rsq);
  else 
    return TVector2(0,0);
  return r;
}

double Engine::GetSmoothingKernelLapl(const unsigned int i, const unsigned int j) {
  double const x1 = fluid->x->at(i);
  double const y1 = fluid->x->at(i);
  double const x2 = fluid->x->at(j);
  double const y2 = fluid->x->at(j);
  double const rsq = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
  double const hsq = _NG_H_*_NG_H_;
  double result = 0;
  if(rsq <= hsq) {
    result = -12.*alpha*(hsq-rsq)*(hsq-3*rsq);
  }
  return result;
}

TVector2 Engine::GetSurfaceNormal(const unsigned int i) {
  TVector2 norm(0,0);
  std::vector<unsigned int> indices = htable->FindNN(TVector2(
    fluid->x->at(i),
    fluid->y->at(i)
  ));

  if(indices.size() < 4)
    return norm;

  for(unsigned int hindice = 0, j = 0; hindice < indices.size(); ++hindice) {
    j = indices[hindice];
    norm += fluid->m->at(j)/fluid->rho->at(j)*GetSmoothingKernelGrad(i,j);
  }
  return 1.0*norm.Unit();
}


TVector2 Engine::GetArtificialForce(const unsigned int i, const unsigned int j) {
  double const magnitude = 1e7;
  double const x1 = fluid->x->at(i);
  double const y1 = fluid->y->at(i);
  double const x2 = fluid->x->at(j);
  double const y2 = fluid->y->at(j);
  TVector2 r(x2-x1, y2-y1);
  r = r.Unit();
  double const rlen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

  double const px = rlen/_NG_H_*3.;

  if(px < 1.53)
    return 0*r;

  double const p1                        =     -7.82904;
  double const p2                        =      6.18333;
  double const p3                        =     -1.51929;
  double const p4                        =     0.109357;

  return magnitude * (p1+2*p2*px+3*p3*px*px+4*p4*px*px*px+0.0/9.0/rlen/rlen)*r;
}