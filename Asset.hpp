#ifndef ASSET_HPP
#define ASSET_HPP
#include<string>
#include<cctype> 
#include<iostream>

using std::string;

class Asset {
public:
    Asset();
    Asset( const Asset& );
    ~Asset();
    void setName(string);
    string getName();
    double& Rate();
    double& InitialValue();
    double& Value();
protected:
    double initialValue_, value_, r_;
    string name_;
};

#endif

