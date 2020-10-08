#ifndef sipm_h
#define sipm_h

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//---------------------------------Initial declaaration of functions----------------------------------//

vector< vector<double> > getData(string file, char delim, int start);
double EE(int exponentPassed);
Double_t poissonf(Double_t*x,Double_t*par);

//----------------------------------------------------------------------------------------------------//



//--------------------------------------Data retireval function---------------------------------------//

vector< vector<double> > getData(string file, char delim, int start) {

  //-------------------------------Push function parameters to variables--------------------------------//

  string fileName = file;
  char delimiter = delim;
  int startLine = start;

  //----------------------------------------------------------------------------------------------------//



  //------------------------------Open stream of file with given filename-------------------------------//

  ifstream ifs(fileName.c_str());

  //----------------------------------------------------------------------------------------------------//



  //--------------------------------------Variable initialization---------------------------------------//

  vector< vector<double> > dataSet;     //Vector for full dataset, each element is a vector of elements seperated by given delimiter
  vector<double> row;                   //Each row of dataset, elements are all values that were seperated by given delimiter
  string lineString;                    //String that each row is pushed to before being seperated bby given delimiter
  string elementString;                 //String that each element of a given line is pushed to before being added to row vector
  int lineCount = 0;                    //Integer to keep track of position in file, used to skip header rows if necessary

  while( getline(ifs, lineString) ) {

    //Increase lineCount variable
    lineCount ++;

    //Clear row vector for new row
    row.clear();

    //Add current line to lineString variable
    stringstream ss(lineString);

    //Split lineString into elements one by one, convert to double and add each to row vector
    while(getline(ss, elementString, delimiter)){
      row.push_back( strtod(elementString.c_str(), NULL) );
    }

    //Add row vector to dataSet vector, skip header rows if necessary
    if (lineCount >= startLine) {
      dataSet.push_back(row);
    }
  }
  return dataSet;

  //----------------------------------------------------------------------------------------------------//
}


Double_t poissonf(Double_t*x,Double_t*par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

//----------------------------------------------------------------------------------------------------//



//-----------------Scientific notation function, for simplifying nomenclature in code-----------------//

double EE(int exponentPassed) {
  int exponent = exponentPassed;
  return pow(10,exponent);
}

//----------------------------------------------------------------------------------------------------//

#endif
