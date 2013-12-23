#include <iostream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <Particle.hpp>

int main() {

  const int nSteps = 1e3;

  TFile *initial = TFile::Open("./fluids/initial.root","READ");
  TTree *initialSets = 0;
  
  TBranch *xReadBranch = 0;
  TBranch *yReadBranch = 0;
  TBranch *vxReadBranch = 0;
  TBranch *vyReadBranch = 0;
  TBranch *mReadBranch = 0;
  TBranch *rhoReadBranch = 0;

  std::vector<double> *x = 0;
  std::vector<double> *y = 0;
  std::vector<double> *vx = 0;
  std::vector<double> *vy = 0;
  std::vector<double> *m = 0;
  std::vector<double> *rho = 0;

  std::vector<double> *xComputed = 0;
  std::vector<double> *yComputed = 0;
  std::vector<double> *vxComputed = 0;
  std::vector<double> *vyComputed = 0;
  std::vector<double> *mComputed = 0;
  std::vector<double> *rhoComputed = 0;

  initial->GetObject("sets",initialSets);
  initialSets->SetBranchAddress("x",&x,&xReadBranch);
  initialSets->SetBranchAddress("y",&y,&yReadBranch);
  initialSets->SetBranchAddress("vx",&vx,&vxReadBranch);
  initialSets->SetBranchAddress("vy",&vy,&vyReadBranch);
  initialSets->SetBranchAddress("m",&m,&mReadBranch);
  initialSets->SetBranchAddress("rho",&rho,&rhoReadBranch);

  TFile *output = TFile::Open("./fluids/output.root","RECREATE");
  TTree *sets = new TTree("sets", "Fluid particles sets");
  sets->Branch("x",&xComputed);
  sets->Branch("y",&yComputed);
  sets->Branch("vx",&vxComputed);
  sets->Branch("vy",&vyComputed);
  sets->Branch("m",&mComputed);
  sets->Branch("rho",&rhoComputed);  

  Long64_t entryNumber = initialSets->LoadTree(0);
  xReadBranch->GetEntry(entryNumber);
  yReadBranch->GetEntry(entryNumber);
  vxReadBranch->GetEntry(entryNumber);
  vyReadBranch->GetEntry(entryNumber);
  mReadBranch->GetEntry(entryNumber);
  rhoReadBranch->GetEntry(entryNumber);

  for(unsigned int i = 0; i < nSteps; ++i) {
    xComputed = new std::vector<double>(*x);
    yComputed = new std::vector<double>(*y);
    vxComputed = new std::vector<double>(*vx);
    vyComputed = new std::vector<double>(*vy);
    mComputed = new std::vector<double>(*m);
    rhoComputed = new std::vector<double>(*rho);
    for(unsigned int j = 0; j < xComputed->size(); ++j) {
      xComputed->at(j) = xComputed->at(j) + 50./nSteps;
      yComputed->at(j) = yComputed->at(j) - 50./nSteps;
    }
    sets->Fill();
    x = new std::vector<double>(*xComputed);
    y = new std::vector<double>(*yComputed);
    vx = new std::vector<double>(*vxComputed);
    vy = new std::vector<double>(*vyComputed);
    m = new std::vector<double>(*mComputed);
    rho = new std::vector<double>(*rhoComputed);

    delete xComputed;
    delete yComputed;
    delete vxComputed;
    delete vyComputed;
    delete mComputed;
    delete rhoComputed;
  }

  output->Write();
  delete output;
  // delete sets;

  delete initial;

  return 0;
}