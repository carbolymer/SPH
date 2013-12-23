#ifndef _FLUID_HPP_
#define _FLUID_HPP_

#include <iostream>
#include <vector>
#include <TTree.h>
#include <TBranch.h>

class Fluid {
private:
  TTree *readTree;
  TTree *writeTree;
  TBranch *xReadBranch;
  TBranch *yReadBranch;
  TBranch *vxReadBranch;
  TBranch *vyReadBranch;
  TBranch *mReadBranch;
  TBranch *rhoReadBranch;  
public:
  std::vector<double> *x;
  std::vector<double> *y;
  std::vector<double> *vx;
  std::vector<double> *vy;
  std::vector<double> *m;
  std::vector<double> *rho;
  Fluid();
  ~Fluid(){};
  void GetEntry(Long64_t entryNumber);
  void InstallReadHooks(TTree *readTree);
  void InstallWriteHooks(TTree *writeTree);
  void Reset();
  void AllocateSpace();
  Fluid& operator=(Fluid &rhs);
};

#endif