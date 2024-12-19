// Member function definitions for Asset class

#include "Asset.hpp"

// Constructor initalizes all data members to zero.
Asset::Asset() { r_ = initialValue_ = value_ = 0; }


// Copy Constructor
Asset::Asset( const Asset& other){
    initialValue_ = other.initialValue_;
    value_ = other.value_;
    name_ = other.name_;
    r_ = other.r_;
}

// Destructor
Asset::~Asset(){}

// name
void Asset::setName( string x ){
    for( int i = 0 ; i < x.size() ; i++ ) {
        x[i] = toupper( x[i] );
        name_ = x;
    }
}
string Asset::getName() { return name_; }

// Interest Rate
double& Asset::Rate() { return r_; }

// initialValue
double& Asset::InitialValue() { return initialValue_; }

// Price of the asset
double& Asset::Value() { return value_; }
