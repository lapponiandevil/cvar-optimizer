#ifndef NORMCDF_HPP
#define NORMCDF_HPP

#include <cmath>

static const double b1 =  0.319381530,
					b2 = -0.356563782,
					b3 =  1.781477937,
					b4 = -1.821255978,
					b5 =  1.330274429,
					p  =  0.2316419,
					c  =  0.39894228;

double Normcdf(const double);

#endif