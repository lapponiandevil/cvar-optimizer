#ifndef SCENARIOS_HPP
#define SCENARIOS_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>

#include "dSFMT.h"
#include "normsinv.h"
#include "Matrix.hpp"
#include "Portfolio.hpp"
#include "Asset.hpp"
#include "Share.hpp"
#include "Option.hpp"

using namespace std;

class Scenarios {
public:
	Scenarios();
	Scenarios(const Scenarios& );
	~Scenarios();
	void clearScenarios();
	Matrix<double> CVaRScen(Portfolio&, double, int);
	Matrix<double> DDScen(Portfolio&, double, int, int);
	
private:
	bool endValInitiated_, DDValInitiated_;
	int i,j,J,k,l,m,n,o,size,seed,shareIndex,tSteps_;
	double T, val_, r_, dRate_, randArray_[];
	vector<double>  aVec_, bVec_, cVec_;
	Matrix<double> scenarioMatrix_, DD_Matrix_, randMatrix_;
	dsfmt_t randGen_;

	vector<double> tVec_;
	vector< Matrix<double>* > tMatrixVector_;
	
	Matrix<double> *mTmp_;
};

#endif