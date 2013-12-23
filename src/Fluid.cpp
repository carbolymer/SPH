#include <Fluid.hpp>

Fluid::Fluid() : 
  readTree(0),
  writeTree(0),
  xReadBranch(0),
  yReadBranch(0),
  vxReadBranch(0),
  vyReadBranch(0),
  mReadBranch(0),
  rhoReadBranch(0),
  x(0),
  y(0),
  vx(0),
  vy(0),
  m(0),
  rho(0) {
  ;
}


void Fluid::InstallReadHooks(TTree *tree) {
  readTree = tree;
  readTree->SetBranchAddress("x",&x,&xReadBranch);
  readTree->SetBranchAddress("y",&y,&yReadBranch);
  readTree->SetBranchAddress("vx",&vx,&vxReadBranch);
  readTree->SetBranchAddress("vy",&vy,&vyReadBranch);
  readTree->SetBranchAddress("m",&m,&mReadBranch);
  readTree->SetBranchAddress("rho",&rho,&rhoReadBranch);
}

void Fluid::InstallWriteHooks(TTree *tree) {
  writeTree = tree;
  writeTree->Branch("x",&x);
  writeTree->Branch("y",&y);
  writeTree->Branch("vx",&vx);
  writeTree->Branch("vy",&vy);
  writeTree->Branch("m",&m);
  writeTree->Branch("rho",&rho);
}

void Fluid::GetEntry(Long64_t entryNumber) {
  xReadBranch->GetEntry(entryNumber);
  yReadBranch->GetEntry(entryNumber);
  vxReadBranch->GetEntry(entryNumber);
  vyReadBranch->GetEntry(entryNumber);
  mReadBranch->GetEntry(entryNumber);
  rhoReadBranch->GetEntry(entryNumber);
}

void Fluid::Reset() {
  delete x;
  delete y;
  delete vx;
  delete vy;
  delete m;
  delete rho;
  x = 0;
  y = 0;
  vx = 0;
  m = 0;
  rho = 0;
}

Fluid& Fluid::operator=(Fluid &rhs) {
  x = new std::vector<double>(*(rhs.x));
  y = new std::vector<double>(*(rhs.y));
  vx = new std::vector<double>(*(rhs.vx));
  vy = new std::vector<double>(*(rhs.vy));
  m = new std::vector<double>(*(rhs.m));
  rho = new std::vector<double>(*(rhs.rho));
  return *this;
}