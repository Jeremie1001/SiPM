// @(#)root

//#include <cstdio>
#include <iostream>
//#include <iomanip>
//#include <fstream>
#include <string>
//#include <sstream>
//#include "TFile.h"
#include <vector>
#include <math.h>

using namespace std;

vector< vector<double> > getData(string file, char delim) {
  string fileName = file;
  char delimiter = delim;

  ifstream ifs(fileName);

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
      row.push_back(stod(entry));
    }
    data.push_back(row);
  }
  return data;
}

void gainFit() {
  string fileName = "../Data/100att_gain32.csv";
  vector< vector<double> > data = getData(fileName, ',');

  TCanvas *c1 = new TCanvas();

/*
  TH1D *hist;
  hist = new TH1D("h1","histogram 1",data.size(),data.front()[0],data.back()[0]);

  for(int i = 0; i < data.size(); i++) {
    hist->SetBinContent(i, data[i][1]);
    cout<<"Setting bin "<<i<<" as: "<<hist->GetBinContent(i)<<endl;
  }
  hist->Draw();
*/
///*

  Double_t x[data.size()];
  Double_t y[data.size()];

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
//*/
}
