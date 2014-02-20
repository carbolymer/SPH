// requires ROOT (root.cern.ch)

int potentialFitter() {
  double xPoints[] = {
    0,
    // 0.75,
    0.5,
    1.9,
    3,
    3.1
  };
  double yPoints[] = {
    0,
    // -2.78,
    -3,
    -2,
    0,
    0.02
  };

  TF1 *fit = new TF1("fit","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x",0,3.2);
  fit->SetParameter(0,1);
  fit->SetParameter(1,1);
  fit->SetParameter(2,1);
  fit->SetParameter(3,1);
  fit->SetParameter(4,1);

  TGraph *potential = new TGraph(5, xPoints, yPoints);
  potential->Draw("AC*");

  potential->Fit("fit");

  return 0;
}