// @(#)root

#include <cstdlib>
#include <TFile.h>
#include "SiPM.h"

void fitter(TF1 *f, int start, int end) {

}

void gainFit() {
  string fileName = "/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Data/100att_gain32.csv";

  vector< vector<double> > data = getData(fileName, ',', 0);

  TCanvas *c1 = new TCanvas();

  unsigned int const size = data.size();

  Double_t x[size];
  Double_t y[size];

  for(int i = 0; i < data.size(); i++) {
    x[i] = data[i][0];
    y[i] = data[i][1];
  }

  int gaussLimits[11][2] = {{22,68},{76,123},{124,178},{180,230},{238,284},{295,339},{346,394},{402,451},{456,508},{514,564},{567,621}};
  int gaussFitNum = sizeof(gaussLimits)/sizeof(gaussLimits[0]);
  double gaussResults[11][3][2];

  TGraph* gr;
  gr = new TGraph(data.size(),x,y);

  //TF1 *g1;

  for(int i = 0; i < gaussFitNum; i++) {
    TF1 *g1 = new TF1("m1","gaus",x[gaussLimits[i][0]],x[gaussLimits[i][1]]);
    gr->Fit("m1","QR+");
    cout<<i<<":"<<endl;
    for (int j = 0; j < 3; j++) {
      gaussResults[i][j][0] = g1->GetParameter(j);
      gaussResults[i][j][1] = g1->GetParError(j);

      string paramName = "";
      if (j == 0) {
        paramName = "Peak";
      } else if (j == 1) {
        paramName = "Mean";
      } else if (j == 2) {
        paramName = "Sigma";
      }

      cout<<paramName<<": "<<gaussResults[i][j][0]<<" +/- "<<gaussResults[i][j][1]<<endl;
    }
    cout<<endl;
    g1->SetLineWidth(1);
  }

  gr->SetLineWidth(0);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->SetTitle("Gain Plot");
  gr->GetXaxis()->SetTitle("Detected");
  gr->GetYaxis()->SetTitle("Channel");
  gr->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/gainFit.png");
}
