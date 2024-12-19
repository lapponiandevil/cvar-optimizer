// Member function definitions for Option class

#include "Option.hpp"


// Constructor initalizes all data members to zero.
Option::Option() {}

// Copy Constructor
Option::Option( const Option& other) {
    initialValue_ = other.initialValue_;
    value_ = other.value_;
    name_ = other.name_;
    strike_ = other.strike_;
    T_ = other.T_;
    underlying_ = other.underlying_;
    type_ = other.type_;
}

// Destructor
Option::~Option(){}

// Name Of Underlying Share
Share& Option::Underlying(){ return underlying_; }

// Type of Option
void Option::setType( string x ){
    for( int i_ = 0 ; i_ < x.size(); i_++ ) {
       x[i_] = toupper( x[i_] );
    }
    type_ = x;
}
string Option::getType() { return type_; }

// Strike
double& Option::Strike() { return strike_; }

// Maturity
double& Option::Maturity() { return T_; }

// Value Of a Standard European Call
void Option::calcValue( double t_, double r ) {
    vol_ = underlying_.Volatility();
    S    = underlying_.Value();	
    if ( t_ < T_ ) {
	    d1_ = (log(S/strike_)+(r+pow(vol_,2)/2)*(T_-t_))/(vol_*sqrt(T_-t_));
    	d2_ = d1_-vol_*sqrt(T_-t_);
		if( type_ == "CALL" ) {
        	value_ = (S*Normcdf(d1_) -strike_*exp(-r*(T_-t_))*Normcdf(d2_));
    	}
    	if( type_ == "PUT" ) {
        	value_ = (strike_*exp(-r*(T_-t_))*Normcdf(-d2_) -S*Normcdf(-d1_) );
    	}
    }
	else {
    	if( type_ == "CALL" ) {
        	value_ = (S -strike_);
    	}
    	if( type_ == "PUT" ) {
        	value_ = (strike_ -S);
    	}
    }
    value_ = ( value_ > 0? value_ : 0 );
}
