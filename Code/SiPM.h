#ifndef sipm_h
#define sipm_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector< vector<double> > getData(string file, char delim, int start);
double pythagoreanTheorem(vector<double> numbers);

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

#endif
