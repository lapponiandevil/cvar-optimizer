// Member function definitions for Share class

#include "Share.hpp"

// Constructor initalizes all data members to zero.
Share::Share() { value_ = vol_ = r_ = 0; }

// Copy Constructor
Share::Share( const Share& other)
{
    initialValue_ = other.initialValue_;
    value_ = other.value_;
    name_ = other.name_;
    vol_ = other.vol_;
    r_ = other.r_;
}

// Destructor
Share::~Share(){}

// Volatility
double& Share::Volatility() { return vol_; }

void Share::simValue( const double T, const double X )
{
    value_ = initialValue_*exp( ( r_-pow(vol_,2)/2 )*T +vol_*X*sqrt(T) );
}
