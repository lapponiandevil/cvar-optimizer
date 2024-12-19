#ifndef CVAR_HPP
#define CVAR_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

#include "Matrix.hpp"
#include "Asset.hpp"
#include "Share.hpp"
#include "Option.hpp"
#include "Portfolio.hpp"
#include "Scenarios.hpp"
#include "writeToFile.hpp"
#include "Parameters.hpp"

using namespace std;

class CVaR {
		
public:
	CVaR();
	CVaR( const CVaR& );
	~CVaR();
	double Optimize( Portfolio&, Scenarios&, writeToFile&, Parameters& );

private:
	int     i, j, m, n, o,index_,shareIndex;
	double 	incr_,	// Size of increment in efficient frontier (optional).
			VaR_,	// Extracted after optimization.
			R_,		// Return of Portfolio.
			nu_,	// Maximum percentage of portfolio
					// for each individual asset. (1=100%)
			val_;
	char name_;
	string temp_;
	vector<double> 	PL_Vec_,
					expectedValue_;

	writeToFile		toFile_; // Used to write to different files.
	Matrix<double>	scenarioMatrix_;

};

#endif
