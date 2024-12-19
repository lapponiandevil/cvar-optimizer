#ifndef STOCK_HPP
#define STOCK_HPP   
#include <cmath> 
#include "Asset.hpp"

using std::string;

class Stock : public Asset {
public:
    Stock();
    Stock( const Stock& );
    ~Stock();
    double& Volatility();
    void simValue( const double, const double );
private:
    double vol_;
};

#endif
