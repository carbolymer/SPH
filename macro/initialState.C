// requires ROOT (root.cern.ch)

#include <iostream>
#include <vector>
#include <TMath.h>
#include <TH2D.h>
#include <TF2.h>
#include <TFile.h>

Double_t GetSpatialDistribution(Double_t *x, Double_t *par) {
  Double_t const radius = par[0];
  Double_t const xmean = par[1];
  Double_t const ymean = par[2];
  Double_t const xsigma = par[3];
  Double_t const ysigma = par[4];
  Double_t const r = TMath::Sqrt((x[0]-xmean)*(x[0]-xmean)+(x[1]-ymean)*(x[1]-ymean));
  // return TMath::Gaus(x[0], xmean, xsigma)*TMath::Gaus(x[1], ymean, ysigma);
  return 1/(TMath::Exp(xsigma*(r-radius))+1);
}

int initialState() {

  const int nParticles = 2000;

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
  spatialDistribution->SetParameter(3,3);  // sigmax
  spatialDistribution->SetParameter(4,3);  // sigmay

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
    // vx.push_back(1e3.*(y_-x_));
    // vy.push_back(1e3.*(-x_-y_+100));
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