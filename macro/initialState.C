// requires ROOT (root.cern.ch)

#include <iostream>
#include <vector>
#include <TMath.h>
#include <TH2D.h>
#include <TF2.h>
#include <TFile.h>

Double_t GetSpatialDistribution(Double_t *x, Double_t *par) {
  Double_t radius = par[0];
  Double_t xmean = par[1];
  Double_t ymean = par[2];
  Double_t xsigma = par[3];
  Double_t ysigma = par[4];
  Double_t r = x[0]*x[0]+x[1]*x[1];
  return TMath::Gaus(x[0], xmean, xsigma)*TMath::Gaus(x[1], ymean, ysigma);
}

int initialState() {

  const int nParticles = 1000;

  TFile *initial = TFile::Open("./fluids/initial.root","RECREATE");
  TTree *sets = new TTree("sets", "Fluid particles sets");

  gRandom = new TRandom3(time(NULL));

  Double_t x_, y_;
  
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> vx;
  std::vector<double> vy;
  std::vector<double> m;
  std::vector<double> rho;

  TH2D *fluid = new TH2D("fluid","fluid",400, 0, 100, 400, 0, 100);
  
  TF2* spatialDistribution = new TF2("spatial", GetSpatialDistribution, 0, 100, 0, 100, 5);
  spatialDistribution->SetNpx(100);
  spatialDistribution->SetNpy(100);
  spatialDistribution->SetParameter(0,20); // radius
  spatialDistribution->SetParameter(1,50); // position of xmean
  spatialDistribution->SetParameter(2,50); // position of ymean
  spatialDistribution->SetParameter(3,5);  // sigmax
  spatialDistribution->SetParameter(4,5);  // sigmay

  sets->Branch("x",&x);
  sets->Branch("y",&y);
  sets->Branch("vx",&vx);
  sets->Branch("vy",&vy);
  sets->Branch("m",&m);
  sets->Branch("rho",&rho);

  x.clear();
  y.clear();
  vx.clear();
  vy.clear();
  rho.clear();
  m.clear();
  
  for(int i = 0; i < nParticles; ++i) {
    spatialDistribution->GetRandom2(x_,y_);
    fluid->Fill(x_,y_);
    x.push_back(x_);
    y.push_back(y_);
    vx.push_back(0.);
    vy.push_back(0.);
    m.push_back(1);
    rho.push_back(1);
  }

  sets->Fill();
  initial->Write();
  delete initial;

  // fluid->Draw("colz");

  return 0;
}