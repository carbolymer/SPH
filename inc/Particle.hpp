#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#include <cmath>

class Particle : public TObject
{
public:
  Particle() {};
  virtual ~Particle(){};
  double x;
  double y;
  double vx;
  double vy;
  double rho; // density
  double m;
  double GetR() { return sqrt(x*x+y*y); }
  double GetV() { return sqrt(vx*vx+vy*vy); }
};

#endif