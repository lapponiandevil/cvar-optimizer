// Member function definitions for Portfolio class

#include "Portfolio.hpp"

// Constructor initalizes all data members to zero.
Portfolio::Portfolio() {}

// Copy Constructor
Portfolio::Portfolio( const Portfolio& other) {
    assetVec_ = other.assetVec_;
    shareVec_ = other.shareVec_;
    optionVec_ = other.optionVec_;
    assetPositionVec_ = other.assetPositionVec_;
    sharePositionVec_ = other.sharePositionVec_;
    optionPositionVec_ = other.optionPositionVec_;
    initialAssetPositionVec_ = other.initialAssetPositionVec_;
    initialSharePositionVec_ = other.initialSharePositionVec_;
    initialOptionPositionVec_ = other.initialOptionPositionVec_;
}

// Destructor
Portfolio::~Portfolio(){}


// Adds Assets, Shares, Options to Portfolio
void Portfolio::load() {
    
    Asset tempAsset_;
    Share tempShare_;
    Option tempOption_;
    string str_, name_, underl_, type_;
    int index_;
    char ch_;
    double r_ ,initPos_, initVal_, val_, vol_, strike_, maturity_;
    
    ifstream readAssets( "input/assetList.txt" );
    
    if( !readAssets ) {
        cerr << "File could not be opened\n";
        exit(1);
    }
    
    while( !readAssets.eof() ) {
        ch_= 'z';
        readAssets >> ch_;
        ch_ = tolower( ch_ );
        switch (ch_) {
            case 'r': {
                readAssets >>name_ >>riskFRate_;
                break;
            }
            case 'a': {
                readAssets >>name_ >>r_ >>initPos_ >>initVal_;
                tempAsset_.setName(name_);
                tempAsset_.Rate() = r_;
                tempAsset_.InitialValue() = initVal_;
                addAsset(tempAsset_);
                setInitialAssetPosition( getAsset(name_), initPos_);
                break;
            }
            case 's': {
                readAssets >>name_ >>r_ >>vol_ >>initPos_ >>initVal_;
                tempShare_.setName(name_);
                tempShare_.Rate() = r_;
                tempShare_.InitialValue() = initVal_;
                tempShare_.Value() = initVal_;
                tempShare_.Volatility() = vol_;
                addShare(tempShare_);
                setInitialSharePosition( getShare(name_), initPos_);
                break;
            }
            case 'o': {
                readAssets  >>name_ >>underl_ >>type_
                            >>strike_ >>maturity_ >>initPos_;
                tempOption_.setName(name_);
                for( int i = 0 ; i < underl_.size() ; i++ ) {
                    underl_[i] = toupper( underl_[i] );
                }
                tempOption_.Underlying() = getShare(getShare(underl_));
                tempOption_.setType(type_);
                tempOption_.Strike() = strike_;
                tempOption_.Maturity() = maturity_;
                tempOption_.calcValue( 0, riskFRate_);
                tempOption_.InitialValue() = tempOption_.Value();
                addOption(tempOption_);
                setInitialOptionPosition( getOption(name_), initPos_);
                break;
            }
            default : {
            	break;
            }
        }//switch
    } // while
    readAssets.close();
}// load


void Portfolio::clear() {
    assetVec_.clear();
    shareVec_.clear();
    optionVec_.clear();
    assetPositionVec_.clear();
    sharePositionVec_.clear();
    optionPositionVec_.clear();
    initialAssetPositionVec_.clear();
    initialSharePositionVec_.clear();
    initialOptionPositionVec_.clear();
}

// Returns the Risk Free Rate.
double Portfolio::getRiskFreeRate(){
    return riskFRate_;
}

    // Asset Vector
// Add Asset(s) to assetVec
void Portfolio::addAsset( Asset x ){
    assetVec_.push_back ( x );
    assetPositionVec_.push_back ( 0 );
    initialAssetPositionVec_.push_back ( 0 );
}
void Portfolio::addAsset( vector<Asset> x ) {
    for( int i = 0 ; i < x.size() ; i++ ) { addAsset( x[i] ); }
}

// Get Asset From assetVec
Asset& Portfolio::getAsset( int x ) {
    if( x <= assetVec_.size() ) { return assetVec_[x]; }
    else { throw noSuchAssetException(); }
}
int Portfolio::getAsset( string x ) {
    for( int i = 0 ; i < x.size() ; i++ ) {
        x[i] = toupper( x[i] );
    }
    int i = 0;
    while( (assetVec_[i]).getName() != x && i < assetVec_.size()  ) { i++; }
    if( assetVec_[i].getName() != x ) { throw noSuchAssetException(); }
    else { return i; }
}

// Returns The Number Of Assets
int Portfolio::numberOfAssets(){ return assetVec_.size(); }

// Asset Positions
// New-
void Portfolio::setAssetPosition(int i , double x){ assetPositionVec_[i] = x; }
double Portfolio::getAssetPosition( int i ){ return assetPositionVec_[i]; }
// Initial-
void Portfolio::setInitialAssetPosition( int i, double x ) {
    initialAssetPositionVec_[i] = x;
}
double Portfolio::getInitialAssetPosition( int i ) {
    return initialAssetPositionVec_[i];
}


    // Share Vector
// Add Share(s) to shareVec
void Portfolio::addShare( Share x ){
    shareVec_.push_back ( x );
    sharePositionVec_.push_back ( 0 );
    initialSharePositionVec_.push_back ( 0 );
}
void Portfolio::addShare( vector<Share> x ) {
    for( int i = 0 ; i < x.size() ; i++ ) { addShare( x[i] ); }
}

// Get Share From shareVec
Share& Portfolio::getShare( int x ){ return shareVec_[x]; }
int Portfolio::getShare( string x ) {
    for( int i = 0 ; i < x.size() ; i++ ) {
        x[i] = toupper( x[i] );
    }
    int i = 0;
    while( i < shareVec_.size() ) {
        if( shareVec_[i].getName() == x ) { break; }
        i++;
    }
    
    if( shareVec_[i].getName() != x ) { throw noSuchAssetException(); }
    else{ return i; }
}

// Returns The Number Of Shares
int Portfolio::numberOfShares(){ return shareVec_.size(); }

// Share Positions
// New-
void Portfolio::setSharePosition( int i, double x ){
    sharePositionVec_[ i ] = x;
}
double Portfolio::getSharePosition( int i ){
    return sharePositionVec_[ i ];
}
// Initial-
void Portfolio::setInitialSharePosition( int i, double x ){
    initialSharePositionVec_[ i ] = x;
}
double Portfolio::getInitialSharePosition( int i ){
    return initialSharePositionVec_[ i ];
}



    // Option Vector
// Add Option(s) to optionVec
void Portfolio::addOption( Option x ) {
    optionVec_.push_back ( x );
    optionPositionVec_.push_back ( 0 );
    initialOptionPositionVec_.push_back ( 0 );
}
void Portfolio::addOption( vector<Option> x ) {
    for( int i = 0 ; i < x.size() ; i++ ) { addOption( x[i] ); }
}

// Get Option From optionVec
Option& Portfolio::getOption( int x ) {
    return optionVec_[x];
}
int Portfolio::getOption( string x ) {
    for( int i = 0 ; i < x.size() ; i++ ) {
        x[i] = toupper( x[i] );
    }
    int i = 0;
    while( (optionVec_[i]).getName() != x && i < optionVec_.size()  ) { i++; }
    if( optionVec_[i].getName() != x ) { throw noSuchAssetException(); }
    else { return i; }
}

// Returns The Number Of Options
int Portfolio::numberOfOptions() { return optionVec_.size(); }

// Option Positions
// New-
void Portfolio::setOptionPosition( int i, double x ) {
    optionPositionVec_[ i ] = x;
}
double Portfolio::getOptionPosition( int i ) {
    return optionPositionVec_[ i ];
}
// Initial-
void Portfolio::setInitialOptionPosition( int i, double x ) {
    initialOptionPositionVec_[ i ] = x;
}
double Portfolio::getInitialOptionPosition( int i ) {
    return initialOptionPositionVec_[ i ];
}


