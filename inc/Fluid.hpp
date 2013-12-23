#ifndef _FLUID_HPP_
#define _FLUID_HPP_

#include <iostream>
#include <vector>

class Fluid {
private:
  TTree* readTree;
  TTree* writeTree;
public:
  std::vector<double> *x;
  std::vector<double> *y;
  std::vector<double> *vx;
  std::vector<double> *vy;
  std::vector<double> *m;
  std::vector<double> *rho;
  void InstallReadHooks(TTree *readTree);
  void InstallWriteHooks(TTree *writeTree);
  void Reset();
  void AllocateSpace();
};

#endif