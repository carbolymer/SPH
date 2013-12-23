#include <iostream>
#include <thread>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include "Fluid.hpp"
#include "Config.hpp"
#include "Benchmark.hpp"
#include "Engine.hpp"

using namespace std; 

int main() {

  const int nSteps = _NUMBER_OF_STEPS_;

  TFile *initial = TFile::Open("./fluids/initial.root","READ");
  TTree *initialSets = 0;

  Fluid fluid;
  Fluid fluidComputed;

  initial->GetObject("sets",initialSets);
  fluid.InstallReadHooks(initialSets);

  TFile *output = TFile::Open("./fluids/output.root","RECREATE");
  TTree *sets = new TTree("sets", "Fluid particles sets");
  fluidComputed.InstallWriteHooks(sets);

  Long64_t entryNumber = initialSets->LoadTree(0);
  fluid.GetEntry(entryNumber);

  Engine::CalculateRetardedVelocity(fluid);
  for(unsigned int i = 0; i < nSteps; ++i) {
    fluidComputed = fluid;
    Benchmark::Tick(i+1,nSteps);

    Engine::PerformComputations(0, fluid.x->size(), fluid, fluidComputed);
    sets->Fill();
    fluid.Reset();
    fluid = fluidComputed;
    fluidComputed.Reset();
  }

  output->Write();
  delete output;

  delete initial;

  return 0;
}