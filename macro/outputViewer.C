#include <iostream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <unistd.h>

int outputViewer() {

  const int nSteps = 1e3;
  const int nSkip = 20;

  Long64_t entryNumber;

  TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",900,900);
  TH2D *histo = new TH2D("histo","fluid",200, 0, 100, 200, 0, 100);

  TFile *output = TFile::Open("./fluids/output.root","READ");
  TTree *outputSets = 0;
  
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

  output->GetObject("sets",outputSets);
  outputSets->SetBranchAddress("x",&x,&xReadBranch);
  outputSets->SetBranchAddress("y",&y,&yReadBranch);
  outputSets->SetBranchAddress("vx",&vx,&vxReadBranch);
  outputSets->SetBranchAddress("vy",&vy,&vyReadBranch);
  outputSets->SetBranchAddress("m",&m,&mReadBranch);
  outputSets->SetBranchAddress("rho",&rho,&rhoReadBranch); 

  for(int i = 0; i < outputSets->GetEntries(); ++i) {
    entryNumber = outputSets->LoadTree(i);
    xReadBranch->GetEntry(entryNumber);
    yReadBranch->GetEntry(entryNumber);
    vxReadBranch->GetEntry(entryNumber);
    vyReadBranch->GetEntry(entryNumber);
    mReadBranch->GetEntry(entryNumber);
    rhoReadBranch->GetEntry(entryNumber);

    if (i && (i%nSkip) == 0) {
      histo->Reset();
      for(unsigned int j = 0; j < x->size(); ++j) {
        histo->Fill(x->at(j),y->at(j));
      }
      if (i == nSkip) {
        histo->Draw("col");
      }
      c1->Modified();
      c1->Update();
      if (gSystem->ProcessEvents())
        break;
    }    
  }

  delete output;

  return 0;
}