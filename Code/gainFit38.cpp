// @(#)root

#include <cstdlib>
#include <TFile.h>
#include "SiPM.h"

void gainFit38() {

  //------------------------------------Retrieve data from CSV files------------------------------------//

  string fileName = "/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Data/dataset1/100att_gain38.csv";

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

  TGraph* channelCountsTGraph;
  channelCountsTGraph = new TGraph(data.size(),x,y);

  //----------------------------------------------------------------------------------------------------//



  //----Add gauss fit to each peak w/ ranges defined in limits array & push params to results array-----//

  int gaussLimits[10][2] = {{129,237},{238,347},{348,454},{455,559},{560,665},{666,773},{774,878},{879,981},{982,1080},{1081,1175}};
  int gaussFitNum = sizeof(gaussLimits)/sizeof(gaussLimits[0]);
  double gaussResults[10][3][2];

  for(int i = 0; i < gaussFitNum; i++) {
    TF1 *g1 = new TF1("m1","gaus",x[gaussLimits[i][0]],x[gaussLimits[i][1]]);
    channelCountsTGraph->Fit("m1","QR+");
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

  double sumResolution = 0;
  double sumDiff = 0;
  for (size_t i = 0; i < gaussFitNum-1; i++) {
    sumDiff += (gaussResults[i+1][1][0]-gaussResults[i][1][0]);
    sumResolution += (gaussResults[i+1][1][0]-gaussResults[i][1][0])/pow(TMath::Abs(pow((gaussResults[i+1][2][0]),2)-pow((gaussResults[i][2][0]),2)),0.5);
  }
  double avgResolution =  sumResolution/(gaussFitNum-1);
  double avgMeanDiff = sumDiff/(gaussFitNum-1);

  double gain = conversion*avgMeanDiff/electron;

  cout<<"Resolution = "<<avgResolution<<endl;
  cout<<"Gain = "<<gain<<endl;

  unsigned int const sizeGainLinear = gaussFitNum-2;

  Double_t xGainLinear[sizeGainLinear];
  Double_t yGainLinear[sizeGainLinear];
  Double_t exGainLinear[sizeGainLinear];
  Double_t eyGainLinear[sizeGainLinear];

  for(int i = 0; i < sizeGainLinear; i++) {
    xGainLinear[i] = (gaussResults[i+1][1][0]+gaussResults[i][1][0])/2;
    yGainLinear[i] = conversion*(gaussResults[i+1][1][0]-gaussResults[i][1][0])/electron;
    eyGainLinear[i] = conversion/electron*pow((pow(gaussResults[i+1][1][1],2)+pow(gaussResults[i][1][1],2)),0.5);
  }

  TGraphErrors* gainLinearTGraph;
  gainLinearTGraph = new TGraphErrors(sizeGainLinear,xGainLinear,yGainLinear,0,eyGainLinear);

  //----------------------------------------------------------------------------------------------------//



  //----------------------------Fit linear function to Gain v. Channel plot-----------------------------//

  gainLinearTGraph->Fit("pol1", "Q");
  TF1 *f = gainLinearTGraph->GetFunction("pol1");

  double b = f->GetParameter(0);
  double m = f->GetParameter(1);

  double sigb = f->GetParError(0);
  double sigm = f->GetParError(1);

  cout<<"Correlation factor of gain v channel: "<<gainLinearTGraph->GetCorrelationFactor()<<endl;

  //----------------------------------------------------------------------------------------------------//



  //----------------------------------------Gaussian peaks plot-----------------------------------------//

  unsigned int const sizePoisson = gaussFitNum;

  TH1D *gainPoisson;
  gainPoisson = new TH1D("h1", "Histogram for joint datasets", sizePoisson, 0, sizePoisson);

  TF1 *g = new TF1("g","gaus",2000,20000);


  for(int i = 0; i < sizePoisson; i++) {
    g->SetParameters(gaussResults[i][0][0],gaussResults[i][1][0],gaussResults[i][2][0]);
    gainPoisson->SetBinContent(i+1,g->Integral(x[gaussLimits[i][0]],x[gaussLimits[i][1]]));
  }

  gainPoisson->Scale(1/(gainPoisson->Integral()));

  TF1 *funcPoisson = new TF1("poisson",[](double*x,double*p){return TMath::Poisson(x[0],p[0]);},0,20,1);
  TF1 *g2 = new TF1("g2","gaus",0,25);

  gainPoisson->Fit("g2");


  //----------------------------------------------------------------------------------------------------//



  //-------------------------------Draw plots to canvas and save to files-------------------------------//

  TCanvas *c1 = new TCanvas();

  gErrorIgnoreLevel = kWarning;

  channelCountsTGraph->SetLineWidth(0);
  channelCountsTGraph->SetMarkerStyle(20);
  channelCountsTGraph->SetMarkerSize(0.5);
  channelCountsTGraph->SetTitle("Gain Plot");
  channelCountsTGraph->GetXaxis()->SetTitle("Channel");
  channelCountsTGraph->GetYaxis()->SetTitle("Counts");
  channelCountsTGraph->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/gainFit38.png");

  gainLinearTGraph->SetMarkerStyle(20);
  gainLinearTGraph->SetMarkerSize(0.5);
  gainLinearTGraph->SetTitle("Gain Plot");
  gainLinearTGraph->GetXaxis()->SetTitle("Channel");
  gainLinearTGraph->GetYaxis()->SetTitle("Gain");
  gainLinearTGraph->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/gainLinear38.png");

  gainPoisson->SetMarkerStyle(20);
  gainPoisson->SetMarkerSize(1);
  gainPoisson->SetTitle("Poisson Plot");
  gainPoisson->GetXaxis()->SetTitle("N [Peak Number]");
  gainPoisson->GetYaxis()->SetTitle("Counts");
  gainPoisson->Draw();

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/gainPoisson38.png");

  //----------------------------------------------------------------------------------------------------//

}
