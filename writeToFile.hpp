#ifndef WRITETOFILE_HPP
#define WRITETOFILE_HPP
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Parameters.hpp"
#include "Matrix.hpp"
#include "Portfolio.hpp"
#include "Asset.hpp"
#include "Share.hpp"
#include "Option.hpp"

class writeToFile {
public:
	writeToFile();
	writeToFile( const writeToFile& );
	~writeToFile();
	void Lp( Parameters&, Portfolio&, Matrix<double>&, vector<double>& );
	void CVaR_Output ( Matrix<double>& );
	void EF( Matrix<double>&, Parameters& );
	void LaTex ( Portfolio&, Matrix<double>&, Matrix<double>&, Parameters& );

private:
	double round( double, int );
    int i, j, J, m, n, o;
    double sum; // variable used in matrix-building
    vector<string> Methods_;
};

#endif