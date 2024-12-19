#include "Normcdf.hpp"
// Algorithm from:
// http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution

double Normcdf(const double x) {
	if(x >= 0.0) {		double t_ = 1.0 / ( 1.0 + p * x );		return (1.0 - c * exp( -x * x / 2.0 ) * t_ *		( t_ *( t_ * ( t_ * ( t_ * b5 + b4 ) + b3 ) + b2 ) + b1 ));	}	else {		double t_ = 1.0 / ( 1.0 - p * x );		return ( c * exp( -x * x / 2.0 ) * t_ *		( t_ *( t_ * ( t_ * ( t_ * b5 + b4 ) + b3 ) + b2 ) + b1 ));	}}