#ifndef SHARE_HPP
#define SHARE_HPP   
#include <cmath> 
#include "Asset.hpp"

using std::string;

class Share : public Asset {
public:
    Share();
    Share( const Share& );
    ~Share();
    double& Volatility();
    void simValue( const double, const double );
private:
    double vol_;
};

#endif
