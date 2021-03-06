#include <iostream>
#include <thread>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TGraph.h>
#include "Fluid.hpp"
#include "Config.hpp"
#include "Benchmark.hpp"
#include "Engine.hpp"

using namespace std; 

int main() {

  // cout << "dt:\t" << _TIME_STEP_ << endl;
  // cout << "0.25h/c\t" << 0.25*_NG_H_/_NG_C_ << endl;

  const int nSteps = _NUMBER_OF_STEPS_;

  TFile *initial = TFile::Open("./fluids/initial.root","READ");
  TTree *initialSets = 0;

  TCanvas *energyCanvas = new TCanvas("energyCanvas","Energy",1000,800);
  TGraph *energyGraph = new TGraph(_NUMBER_OF_STEPS_);

  Fluid fluid;
  Fluid fluidComputed;
  Engine engine(&fluid);

  initial->GetObject("sets",initialSets);
  fluid.InstallReadHooks(initialSets);

  TFile *output = TFile::Open("./fluids/output.root","RECREATE");
  TTree *sets = new TTree("sets", "Fluid particles sets");
  fluidComputed.InstallWriteHooks(sets);

  Long64_t entryNumber = initialSets->LoadTree(0);
  fluid.GetEntry(entryNumber);

  engine.CalculateRetardedVelocity();
  for(unsigned int i = 0; i < nSteps; ++i) {
    fluidComputed = fluid;
    Benchmark::Tick(i+1,nSteps);

    engine.PerformComputations(0, fluid.x->size(), fluidComputed);
    energyGraph->SetPoint(i+1,i,engine.GetMeanTotalEnergy());

    sets->Fill();
    fluid.Reset();
    fluid = fluidComputed;
    fluidComputed.Reset();
  }

  energyGraph->Draw("AC");
  energyCanvas->SaveAs("./fluids/energy.png");

  output->Write();
  delete output;

  delete initial;

  return 0;
}