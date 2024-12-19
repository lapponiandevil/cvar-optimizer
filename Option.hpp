#ifndef OPTION_HPP
#define OPTION_HPP
#include <cmath>
#include "Normcdf.hpp"
#include "Asset.hpp"
#include "Share.hpp"
#include <iostream>

using std::string;

class Option : public Asset {
public:
    Option();
    Option( const Option& );
    ~Option();
    Share& Underlying();
    void setType( string );
    string getType();
    double& Strike();
    double& Maturity();
    void calcValue( double, double );
    
private:
    string type_;
    double strike_, T_, t_, vol_, S, d1_, d2_;
    Share underlying_;
};

#endif
