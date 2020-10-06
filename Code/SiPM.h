#ifndef sipm_h
#define sipm_h

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector< vector<double> > getData(string file, char delim, int start);
double EE(int exponentPassed);

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

double EE(int exponentPassed) {
  int exponent = exponentPassed;
  return pow(10,exponent);
}

#endif
