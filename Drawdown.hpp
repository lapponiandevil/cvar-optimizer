#ifndef DRAWDOWN_HPP
#define DRAWDOWN_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "Matrix.hpp"
#include "Portfolio.hpp"
#include "Scenarios.hpp"
#include "Parameters.hpp"

using namespace std;

class Drawdown {

public:
	Drawdown();
	Drawdown( const Drawdown& );
	~Drawdown();
	vector<double> Calculate( Portfolio&, Scenarios&, Parameters );
	
private:
	double DD( Parameters );
	double CDaR( double, Parameters );
	double	m,n,o,i,j,k,temp_;
	vector<double> DD_Vec_, tempVec_, returnVec_;
	Matrix<double> DD_Matrix_;
};

#endif