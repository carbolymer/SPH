#include <iostream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <Fluid.hpp>

int main() {

  const int nSteps = 1e3;

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

  for(unsigned int i = 0; i < nSteps; ++i) {
    fluidComputed = fluid;
    for(unsigned int j = 0; j < fluid.x->size(); ++j) {

      fluidComputed.x->at(j) = fluid.x->at(j) + 50./nSteps;
      fluidComputed.y->at(j) = fluid.y->at(j) - 50./nSteps;
      for(unsigned int k = 0; k < fluid.x->size(); ++k);
    }
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