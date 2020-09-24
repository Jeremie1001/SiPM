// @(#)root

#include <cstdlib>
#include <TFile.h>
#include "SiPM.h"

void breakVolt() {
//  string fileName = "../Data/Vbr.csv";
  string fileName = "/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Data/dataset1/Vbr.csv";

  vector< vector<double> > data = getData(fileName, ',', 3);

  TCanvas *c1 = new TCanvas();

  unsigned int const size = data.size();

  Double_t x[size];
  Double_t y[size];

  for(int i = 0; i < data.size(); i++) {
    x[i] = data[i][0];
    y[i] = data[i][1];
  }

  TGraph* gr;
  gr = new TGraph(data.size(),x,y);

  gr->Fit("pol1", "Q");
  TF1 *f = gr->GetFunction("pol1");

  double b = f->GetParameter(0);
  double m = f->GetParameter(1);

  double sigb = f->GetParError(0);
  double sigm = f->GetParError(1);

  double Vbr = -b/m;
  double Vp = Vbr + 3;

  double sig_Vbr = pow((pow((sigb/m),2) + pow((b*sigm/m/m),2)),0.5);
  double sig_Vp = sig_Vbr;

  cout<<"V_br = "<<Vbr<<" +/- "<<sig_Vbr<<" V"<<endl;
  cout<<"V_p = "<<Vp<<" +/- "<<sig_Vp<<" V"<<endl;

  f->SetLineWidth(1);
  gr->SetLineWidth(0);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->SetTitle("Breaking Voltage Plot");
  gr->GetXaxis()->SetTitle("Voltage (V)");
  gr->GetYaxis()->SetTitle("Current (mA)");
  gr->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/breakVolt.png");
}
