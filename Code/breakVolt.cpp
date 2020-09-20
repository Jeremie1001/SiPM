// @(#)root

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib>
#include <TFile.h>

using namespace std;

vector< vector<double> > getData(string file, char delim, int start) {
  string fileName = file;
  char delimiter = delim;
  int startLine = start;

  ifstream ifs(fileName.c_str());

  vector< vector<double> > data;
  vector<double> row;
  string line;
  string entry;

  int linecount = 0;
  while( getline(ifs, line) ) {
    linecount ++;
    row.clear();
    stringstream ss(line);
    while(getline(ss, entry, delimiter)){
      row.push_back( strtod(entry.c_str(), NULL) );
    }
    if (linecount >= startLine) {
      data.push_back(row);
    }
  }
  return data;
}

double pythagoreanTheorem(vector<double> numbers) {
  vector<double> num = numbers;

  double sum = 0;
  for(int i = 0; i < num.size(), i++) {
    sum += pow(num[i], 2);
  }
  sum = pow(sum, 0.5);

  return sum;
}

void breakVolt() {
  string fileName = "../Data/Vbr.csv";
  string fileName = "C:/Users/jerem/Documents/School/Uni/Courses/4th_Year/PHYS4007/SiPM/Data/Vbr.csv";

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
  double Vp = Vbr - 3;

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
  gr->GetYaxis()->SetTitle("Current (mA?)");
  gr->Draw("AP");
}
