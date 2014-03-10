// requires ROOT (root.cern.ch)

#include <iostream>
#include <vector>
#include <TMath.h>
#include <TH2D.h>
#include <TF2.h>
#include <TFile.h>
#include <TTree.h>

TH2D *fluid;
const double xmean = 50;
const double ymean = 50;
const int nParticles = 1000;  

Double_t GetSpatialDistribution(Double_t *x, Double_t *par) {
  Double_t const radius = par[0];
  Double_t const xmean = par[1];
  Double_t const ymean = par[2];
  Double_t const xsigma = par[3];
  Double_t const ysigma = par[4];
  Double_t const r = TMath::Sqrt((x[0]-xmean)*(x[0]-xmean)+(x[1]-ymean)*(x[1]-ymean));
  // return TMath::Gaus(x[0], xmean, xsigma)*TMath::Gaus(x[1], ymean, ysigma);
  return 1/(TMath::Exp(xsigma*(r-radius))+1);
};

void addParticle(
  std::vector<double> &x,
  std::vector<double> &y,
  std::vector<double> &vx,
  std::vector<double> &vy,
  std::vector<double> &m,
  std::vector<double> &rho,
  Double_t x_,
  Double_t y_,
  Double_t vx_,
  Double_t vy_,
  Double_t m_ = 1,
  Double_t rho_ = 1) {
    fluid->Fill(x_,y_);
    x.push_back(x_);
    y.push_back(y_);
    vx.push_back(vx_);
    vy.push_back(vy_);
    m.push_back(m_);
    rho.push_back(rho_);
};

void createDroplet( 
  std::vector<double> &x,
  std::vector<double> &y,
  std::vector<double> &vx,
  std::vector<double> &vy,
  std::vector<double> &m,
  std::vector<double> &rho) {

  gRandom = new TRandom3(time(NULL));
  Double_t x_, y_;

  TF2* spatialDistribution = new TF2("spatial", GetSpatialDistribution, 0, 100, 0, 100, 5);
  spatialDistribution->SetNpx(100);
  spatialDistribution->SetNpy(100);
  spatialDistribution->SetParameter(0,20); // radius
  spatialDistribution->SetParameter(1,xmean); // position of xmean
  spatialDistribution->SetParameter(2,ymean); // position of ymean
  spatialDistribution->SetParameter(3,3);  // sigmax
  spatialDistribution->SetParameter(4,3);  // sigmay

  for(int i = 0; i < nParticles; ++i) {
    spatialDistribution->GetRandom2(x_,y_);
    fluid->Fill(x_,y_);
    x.push_back(x_);
    y.push_back(y_);

    x_ -= xmean;
    y_ -= ymean;

    // rotation + squeeze
    // vx.push_back(1e2.*(y_-x_));
    // vy.push_back(1e2.*(-x_-y_));

    // rotation
    vx.push_back(1e1*(-y_));
    vy.push_back(1e1*(x_));

    // no velocity
    // vx.push_back(0.);
    // vy.push_back(0.);
    m.push_back(1);
    rho.push_back(1);
  }
};

void createDropletCollision( 
  std::vector<double> &x,
  std::vector<double> &y,
  std::vector<double> &vx,
  std::vector<double> &vy,
  std::vector<double> &m,
  std::vector<double> &rho) {

  gRandom = new TRandom3(time(NULL));
  Double_t x_, y_;

  TF2* spatialDistribution = new TF2("spatial", GetSpatialDistribution, 0, 100, 0, 100, 5);
  spatialDistribution->SetNpx(100);
  spatialDistribution->SetNpy(100);
  spatialDistribution->SetParameter(0,15); // radius
  spatialDistribution->SetParameter(1,xmean); // position of xmean
  spatialDistribution->SetParameter(2,ymean+17); // position of ymean
  spatialDistribution->SetParameter(3,3);  // sigmax
  spatialDistribution->SetParameter(4,3);  // sigmay

  // droplet 1
  for(int i = 0; i < nParticles/2; ++i) {
    spatialDistribution->GetRandom2(x_,y_);
    fluid->Fill(x_,y_);
    x.push_back(x_);
    y.push_back(y_);
    vx.push_back(0.);
    vy.push_back(-1e3);
    m.push_back(1);
    rho.push_back(1);
  }

  spatialDistribution->SetParameter(2,ymean-17); // position of ymean
  
  // droplet 2
  for(int i = 0; i < nParticles/2; ++i) {
    spatialDistribution->GetRandom2(x_,y_);
    fluid->Fill(x_,y_);
    x.push_back(x_);
    y.push_back(y_);
    vx.push_back(0.);
    vy.push_back(1e3);
    m.push_back(1);
    rho.push_back(1);
  }

};

void createDiffraction( 
  std::vector<double> &x,
  std::vector<double> &y,
  std::vector<double> &vx,
  std::vector<double> &vy,
  std::vector<double> &m,
  std::vector<double> &rho) {

  // projectile
  addParticle(x,y,vx,vy,m,rho, 41, 50, 1e3, 1e2);
  addParticle(x,y,vx,vy,m,rho, 43, 50, 1e3, 1e2);
  addParticle(x,y,vx,vy,m,rho, 42, 51, 1e3, 1e2);
  addParticle(x,y,vx,vy,m,rho, 42, 49, 1e3, 1e2);

  // target
  addParticle(x,y,vx,vy,m,rho, 51, 50, 0, -1e2);
  addParticle(x,y,vx,vy,m,rho, 50, 51, 0, -1e2);
  addParticle(x,y,vx,vy,m,rho, 49, 50, 0, -1e2);
  addParticle(x,y,vx,vy,m,rho, 50, 49, 0, -1e2);
};

int initialState() {

  TFile *initial = TFile::Open("./fluids/initial.root","RECREATE");
  TTree *sets = new TTree("sets", "Fluid particles sets");
  
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> vx;
  std::vector<double> vy;
  std::vector<double> m;
  std::vector<double> rho;

  fluid = new TH2D("fluid","fluid",400, 0, 100, 400, 0, 100);

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
  
  createDroplet(x,y,vx,vy,m,rho);
  // createDropletCollision(x,y,vx,vy,m,rho);
  // createDiffraction(x,y,vx,vy,m,rho);

  sets->Fill();
  initial->Write();
  // delete initial;

  // fluid->Draw("colz");

  return 0;
}