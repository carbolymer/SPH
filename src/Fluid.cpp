Fluid::Fluid() : x(0), y(0), vx(0), vy(0), m(0), rho(0), readTree(0), writeTree(0){
  ;
}


void Fluid::InstallReadHooks(TTree *tree) {
  ;
}

void Fluid::InstallWriteHooks(TTree *tree) {
  ;
}

void Fluid::Reset() {
  ; // usuwa wszystkie wskazniki
}

void Fluid::AllocateSpace() {
  ; // rezerwuje miejsce przez x = new vector<double>;
}