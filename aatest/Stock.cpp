// Member function definitions for Stock class

#include "Stock.hpp"

// Constructor initalizes all data members to zero.
Stock::Stock() { value_ = vol_ = r_ = 0; }

// Copy Constructor
Stock::Stock( const Stock& other)
{
    initialValue_ = other.initialValue_;
    value_ = other.value_;
    name_ = other.name_;
    vol_ = other.vol_;
    r_ = other.r_;
}

// Destructor
Stock::~Stock(){}

// Volatility
double& Stock::Volatility() { return vol_; }

void Stock::simValue( const double T, const double X )
{
	std::cout << "S " << r_<< std::endl;
    value_ = initialValue_*exp( ( r_-pow(vol_,2)/2 )*T +vol_*X*sqrt(T) );
}
