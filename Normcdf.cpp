#include "Normcdf.hpp"
// Algorithm from:
// http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution

double Normcdf(const double x) {
	if(x >= 0.0) {