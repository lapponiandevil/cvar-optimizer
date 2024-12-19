#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <fstream>

#include "Asset.hpp"
#include "Share.hpp"
#include "Option.hpp"   
#include "noSuchAssetException.hpp"


using namespace std;

class Portfolio {

public:
    Portfolio();
    Portfolio( const Portfolio& );
    ~Portfolio();
    
    void load();
    void clear();
    
    double getRiskFreeRate();
    
    void addAsset( Asset );
    void addAsset( vector<Asset> );
    Asset& getAsset( int );
    int getAsset( string );
    int numberOfAssets();
    void setAssetPosition( int, double );
    double getAssetPosition( int );
    void setInitialAssetPosition( int, double );
    double getInitialAssetPosition( int );
    
    void addShare( Share );
    void addShare( vector<Share> );
    Share& getShare( int );
    int getShare( string );
    int numberOfShares();
    void setSharePosition( int, double );
    double getSharePosition( int );
    void setInitialSharePosition( int, double );
    double getInitialSharePosition( int );
    
    void addOption( Option );
    void addOption( vector<Option> );
    Option& getOption( int );
    int getOption( string );
    int numberOfOptions();
    void setOptionPosition( int, double );
    double getOptionPosition( int );
    void setInitialOptionPosition( int, double );
    double getInitialOptionPosition( int );
    
private:
    double riskFRate_;
    vector<Asset> assetVec_;
    vector<Share> shareVec_;
    vector<Option> optionVec_;
    vector<double> assetPositionVec_;
    vector<double> sharePositionVec_;
    vector<double> optionPositionVec_;
    vector<double> initialAssetPositionVec_;
    vector<double> initialSharePositionVec_;
    vector<double> initialOptionPositionVec_;
};

#endif

