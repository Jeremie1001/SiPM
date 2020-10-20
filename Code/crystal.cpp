// @(#)root

#include <cstdlib>
#include <TFile.h>
#include "SiPM.h"

void crystal() {

  //------------------------------------------Data Processing-------------------------------------------//

  string base = "/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Data/";

  string file[6] = {
    "crystal/bkgd_crystal_histo.csv",
    "crystal/cs_137_crystal_histo.csv",
    "crystal/na_22_crystal_histo.csv"
  };

  TCanvas *c1 = new TCanvas();

  string fileName = base+file[1];

  vector< vector<double> > dataBgd = getData(base+file[0], ',', 0);
  vector< vector<double> > dataCs = getData(base+file[1], ',', 0);
  vector< vector<double> > dataNa = getData(base+file[2], ',', 0);

  //----------------------------------------------------------------------------------------------------//



  //---------------------------------------------Background---------------------------------------------//

  unsigned int const sizeBgd = dataBgd.size();

  Double_t xBgd[sizeBgd];
  Double_t yBgd[sizeBgd];

  for(int i = 0; i < dataBgd.size(); i++) {
    xBgd[i] = dataBgd[i][0];
    yBgd[i] = dataBgd[i][1];
  }

  TGraph* graphBgd;
  graphBgd = new TGraph(dataBgd.size(),xBgd,yBgd);

  //----------------------------------------------------------------------------------------------------//



  //-----------------------------------------------Cesium-----------------------------------------------//

  unsigned int const sizeCs = dataCs.size();

  Double_t xCs[sizeCs];
  Double_t yCs[sizeCs];

  for(int i = 0; i < dataCs.size(); i++) {
    xCs[i] = dataCs[i][0];
    yCs[i] = dataCs[i][1];
  }

  TGraph* graphCs;
  graphCs = new TGraph(dataCs.size(),xCs,yCs);

  TF1 *cesiumFit = new TF1("cesiumFit","gaus",7700,12000);
  graphCs->Fit("cesiumFit", "QR+");

  //----------------------------------------------------------------------------------------------------//



  //-----------------------------------------------Sodium-----------------------------------------------//

  unsigned int const sizeNa = dataNa.size();

  Double_t xNa[sizeNa];
  Double_t yNa[sizeNa];

  for(int i = 0; i < dataNa.size(); i++) {
    xNa[i] = dataNa[i][0];
    yNa[i] = dataNa[i][1];
  }

  TGraph* graphNa;
  graphNa = new TGraph(dataNa.size(),xNa,yNa);

  TF1 *sodiumFit1 = new TF1("sodiumFit1","gaus",5800,9000);
  graphNa->Fit("sodiumFit1", "QR+");

  TF1 *sodiumFit2 = new TF1("sodiumFit2","gaus",15000,24000);
  graphNa->Fit("sodiumFit2", "QR+");

  //----------------------------------------------------------------------------------------------------//



  //---------------------------------------------Linear Fit---------------------------------------------//

  unsigned int const sizeCalibration = 2;

  double yCalibration[sizeCalibration] = {sodiumFit1->GetParameter(1), sodiumFit2->GetParameter(1)};
  double yCalibrationErrors[sizeCalibration] = {sodiumFit1->GetParameter(2), sodiumFit2->GetParameter(2)};

  double xCalibration[sizeCalibration] = {0.511, 1.27};

  TGraphErrors *graphCalibration = new TGraphErrors(sizeCalibration, xCalibration, yCalibration, 0, yCalibrationErrors);

  TF1 *calibrationFit = new TF1("calibrationFit","pol1",0,2);
  graphCalibration->Fit("calibrationFit", "Q");

  double calibrationResults[2][2] = {
    {calibrationFit->GetParameter(0), calibrationFit->GetParError(0)},
    {calibrationFit->GetParameter(1), calibrationFit->GetParError(1)}
  };

  //----------------------------------------------------------------------------------------------------//



  //----------------------------------------------Analysis----------------------------------------------//

  double cesiumChannel[2] = {cesiumFit->GetParameter(1), cesiumFit->GetParameter(2)};
  double cesiumEnergy = (cesiumChannel[0]-calibrationResults[0][0])/calibrationResults[1][0];
  double cesiumEnergyError = pow(pow((cesiumChannel[1]/calibrationResults[1][0]),2) + pow((calibrationResults[0][1]/calibrationResults[1][0]),2) + pow((calibrationResults[1][1]*(cesiumChannel[0]-calibrationResults[0][0])/pow(calibrationResults[1][0],2)),2),0.5);

  cout<<"Cesium Energy: "<<cesiumEnergy<<" +/- "<<cesiumEnergyError<<endl;

  //----------------------------------------------------------------------------------------------------//



  //----------------------------------------------Drawing-----------------------------------------------//

  gErrorIgnoreLevel = kWarning;

  graphBgd->SetLineWidth(0);
  graphBgd->SetMarkerStyle(20);
  graphBgd->SetMarkerSize(0.5);
  graphBgd->SetTitle("Background Spectrum");
  graphBgd->GetXaxis()->SetTitle("Channel");
  graphBgd->GetYaxis()->SetTitle("Counts");
  graphBgd->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/crystalBgdSpectrum.png");

  graphCs->SetLineWidth(0);
  graphCs->SetMarkerStyle(20);
  graphCs->SetMarkerSize(0.5);
  graphCs->SetTitle("Cs-137 Spectrum");
  graphCs->GetXaxis()->SetTitle("Channel");
  graphCs->GetYaxis()->SetTitle("Counts");
  graphCs->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/crystalCesiumSpectrum.png");

  graphNa->SetLineWidth(0);
  graphNa->SetMarkerStyle(20);
  graphNa->SetMarkerSize(0.5);
  graphNa->SetTitle("Na-22 Spectrum");
  graphNa->GetXaxis()->SetTitle("Channel");
  graphNa->GetYaxis()->SetTitle("Counts");
  graphNa->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/crystalSodiumSpectrum.png");

  graphCalibration->SetMarkerStyle(20);
  graphCalibration->SetMarkerSize(0.5);
  graphCalibration->SetTitle("Calibration");
  graphCalibration->GetXaxis()->SetTitle("Energy (MeV)");
  graphCalibration->GetYaxis()->SetTitle("Channel");
  graphCalibration->Draw("AP");

  c1->SaveAs("/home/jeremie1001/Documents/School/Uni/Course/4th_Year/PHYS4007/SiPM/Report/Figures/crytalCalibrationFit.png");

  //----------------------------------------------------------------------------------------------------//

}
