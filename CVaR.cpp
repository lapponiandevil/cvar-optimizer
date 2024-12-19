#include "CVaR.hpp"

//Constructor
CVaR::CVaR() {}

// Copy Constructor
CVaR::CVaR( const CVaR& ot ) {
	m = ot.m;
	n = ot.n;
	o = ot.o;
	index_ = ot.index_;
	shareIndex = ot.shareIndex;
	VaR_ = ot.VaR_;
	R_ = ot.R_;
	nu_ = ot.nu_;
}

// Destructor
CVaR::~CVaR(){}

//------------------------------------------------------------------------------

double CVaR::Optimize(	Portfolio& portf_,
						Scenarios& scen_,
						writeToFile& toFile_,
						Parameters& p ) {

m = portf_.numberOfAssets();
n = portf_.numberOfShares();
o = portf_.numberOfOptions();

//------------------------------------------------------------------------------

// Creates scenarios.
cout << "Creating CVaR scenarios...";
scenarioMatrix_.resize( p.J, m+n+o );
scenarioMatrix_ = scen_.CVaRScen( portf_, p.T_, p.J );
cout << "Done!" << endl;

// Calculates Expected Values Of Scenarios
expectedValue_.resize(m+n+o);
for( i = 0 ; i < m+n+o ; i++ ) {
	for( j = 0 ; j < p.J ; j++ ) {
		expectedValue_[i] += scenarioMatrix_(j,i);
	}
	expectedValue_[i] = expectedValue_[i]/p.J;
}

//------------------------------------------------------------------------------

// Write Problem To File
toFile_.Lp(	p, portf_, scenarioMatrix_, expectedValue_ );

    // Solves Optimization
cout <<"Optimizing portfolio with CVaR constraints...";
system("./lp_solve input/CVaR_model.lp >input/CVaR_opt.txt");
cout << "Done!" << endl;

//------------------------------------------------------------------------------

    // Changes The Portfolio To The Optimal Position
    // By Reading From The Solution
ifstream inFromOutputFile( "input/CVaR_opt.txt" );
if( !inFromOutputFile ) {
    cerr << "File could not be opened\n";
    exit(1);
}
R_=0;

while ( R_ == 0 ) {
	name_ = inFromOutputFile.get();
	if ( name_ == 'T' ) {
		cout << "Problem is infeasible!" << endl;
		goto getOut;
	}
    if ( name_==':' ) {
        inFromOutputFile >> R_;
    }
}
while( inFromOutputFile.good() ) {
    inFromOutputFile >> name_;
    switch (name_) {
        case 'z': {
            goto getOut;
        }
        case 'V': {
            // Temp removes still remaining "a" and "R" from "VaR".
            // val_ reads VaR value.
            inFromOutputFile >> temp_ >> VaR_;
            break;
        }
        case 'x': {
            inFromOutputFile >> index_ >> val_;
            if( abs(val_) < 0.01 ) { val_ = 0; }
            if( index_ < m ) {
                portf_.setAssetPosition( index_, val_ );
            }
            else if( index_ < m+n ) {
                portf_.setSharePosition( index_-m, val_ );
            }
            else if( index_ < m+n+o ) {
                portf_.setOptionPosition( index_-m-n, val_ );
            }
            else { break;}
        }
    }//switch  
} // while
getOut:
inFromOutputFile.close();

R_ *= -1;

/*	// Profit-Loss values.

PL_Vec_.clear();
for( j = 0 ; j < p.J ; j++ ) {
	val_ = 0;
    for( i = 0 ; i < m ; i++ ) {
        val_ += portf_.getAssetPosition(i)*scenarioMatrix_(j,i)
            - portf_.getInitialAssetPosition(i)*
                (portf_.getAsset(i)).InitialValue();
    }
    for( i = 0 ; i < n ; i++ ) {
        val_ += portf_.getSharePosition(i)*scenarioMatrix_(j,m+i)
            - portf_.getInitialSharePosition(i)*
                (portf_.getShare(i)).InitialValue();
    }
    for( i = 0 ; i < o ; i++ ) {
       val_ += portf_.getOptionPosition(i)*scenarioMatrix_(j,m+n+i)
            - portf_.getInitialOptionPosition(i)*
                (portf_.getOption(i)).InitialValue();
    }
    PL_Vec_.push_back( val_ );
}
sort( PL_Vec_.begin(), PL_Vec_.end() );

ofstream outToProfitLoss( "output/PL.txt" );
if( !outToProfitLoss ) {
    cerr << "File could not be opened\n";
    exit(1);
}
for ( j = 0; j < p.J; j++ ) {
	outToProfitLoss << PL_Vec_[j] << endl;
}
outToProfitLoss.close();
*/

return R_;

}//Optimize

