// @(#)root

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib>
#include <TFile.h>

using namespace std;

vector< vector<double> > getData(string file, char delim) {
  string fileName = file;
  char delimiter = delim;
  cout<<fileName<<endl;
  ifstream ifs(fileName.c_str());


  vector< vector<double> > data;
  vector<double> row;
  string line;
  string entry;

  int linecount = 0;
  while(getline(ifs, line))
  {
    linecount ++;
    row.clear();
    stringstream ss(line);
    while(getline(ss, entry, delimiter)){
      row.push_back( strtod(entry.c_str(), NULL) );
    }
    data.push_back(row);
  }
  return data;
}

void gainFit() {
  string fileName = "../Data/100att_gain32.csv";
  string fileName = "C:/Users/jerem/Documents/School/Uni/Courses/4th_Year/PHYS4007/SiPM/Data/100att_gain32.csv";

  vector< vector<double> > data = getData(fileName, ',');

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
  gr->Draw("ACP");
}
