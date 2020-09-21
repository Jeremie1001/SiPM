// @(#)root

#include <stdlib>
#include <TFile.h>
#include "SiPM.h"

void gainFit() {
  string fileName = "../Data/100att_gain32.csv";
  string fileName = "C:/Users/jerem/Documents/School/Uni/Courses/4th_Year/PHYS4007/SiPM/Data/100att_gain32.csv";

  vector< vector<double> > data = getData(fileName, ',', 0);

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
  gr->SetLineWidth(0);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->SetTitle("Gain Plot");
  gr->GetXaxis()->SetTitle("Detected");
  gr->GetYaxis()->SetTitle("Channel");
  gr->Draw("AP");

  c1->SaveAs("C:/Users/jerem/Documents/School/Uni/Courses/4th_Year/PHYS4007/SiPM/Report/Figures/gainFit.png");
}
