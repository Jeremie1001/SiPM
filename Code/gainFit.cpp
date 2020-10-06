// @(#)root

#include <cstdlib>
#include <TFile.h>
#include "SiPM.h"

void gainFit() {

  //------------------------------------Retrieve data from CSV files------------------------------------//

  string fileName = "/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Data/dataset1/100att_gain32.csv";

  vector< vector<double> > data = getData(fileName, ',', 0);

  //----------------------------------------------------------------------------------------------------//



  //--------------------------Create Channel v. Counts plot for gaussian peaks---------------------------//

  unsigned int const size = data.size();

  Double_t x[size];
  Double_t y[size];

  for(int i = 0; i < data.size(); i++) {
    x[i] = data[i][0];
    y[i] = data[i][1];
  }

  //----------------------------------------------------------------------------------------------------//



  //----Add gauss fit to each peak w/ ranges defined in limits array & push params to results array-----//

  int gaussLimits[11][2] = {{22,68},{76,123},{124,178},{180,230},{238,284},{295,339},{346,394},{402,451},{456,508},{514,564},{567,621}};
  int gaussFitNum = sizeof(gaussLimits)/sizeof(gaussLimits[0]);
  double gaussResults[11][3][2];

  TGraph* gr;
  gr = new TGraph(data.size(),x,y);

  for(int i = 0; i < gaussFitNum; i++) {
    TF1 *g1 = new TF1("m1","gaus",x[gaussLimits[i][0]],x[gaussLimits[i][1]]);
    gr->Fit("m1","QR+");
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
    }
    g1->SetLineWidth(1);
  }

  //----------------------------------------------------------------------------------------------------//



  //------------------Get gain for each set of adjacent peaks and plot against channel------------------//

  double conversion = 1.027*EE(-15);
  double electron = 1.602*EE(-19);
  double avgMeanDiff = (gaussResults[(gaussFitNum-1)][1][0]-gaussResults[0][1][0])/(gaussFitNum-1);
  double gain = conversion*avgMeanDiff/electron;

  cout<<"Gain = "<<gain<<endl;

  unsigned int const size2 = gaussFitNum-3;

  Double_t x2[size2];
  Double_t y2[size2];

  for(int i = 0; i < size2; i++) {
    x2[i] = (gaussResults[i+1][1][0]+gaussResults[i][1][0])/2;
    y2[i] = conversion*(gaussResults[i+1][1][0]-gaussResults[i][1][0])/electron;
  }

  TGraph* gr2;
  gr2 = new TGraph((size2),x2,y2);

  //----------------------------------------------------------------------------------------------------//



  //----------------------------Fit linear function to Gain v. Channel plot-----------------------------//

  gr2->Fit("pol1", "Q");
  TF1 *f = gr2->GetFunction("pol1");

  double b = f->GetParameter(0);
  double m = f->GetParameter(1);

  double sigb = f->GetParError(0);
  double sigm = f->GetParError(1);

  cout<<"Correlation factor of gain v channel: "<<gr2->GetCorrelationFactor()<<endl;

  //----------------------------------------------------------------------------------------------------//



  //-------------------------------Draw plots to canvas and save to files-------------------------------//

  TCanvas *c1 = new TCanvas();

  gr->SetLineWidth(0);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->SetTitle("Gain Plot");
  gr->GetXaxis()->SetTitle("Channel");
  gr->GetYaxis()->SetTitle("Counts");
  gr->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/gainFit.png");

  gr2->SetLineWidth(0);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerSize(0.5);
  gr2->SetTitle("Gain Plot");
  gr2->GetXaxis()->SetTitle("Channel");
  gr2->GetYaxis()->SetTitle("Gain");
  gr2->Draw("AP");

  //----------------------------------------------------------------------------------------------------//

}
