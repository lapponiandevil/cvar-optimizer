#include "Drawdown.hpp"

	// Constructor
Drawdown::Drawdown() {}

// Copy Constructor
Drawdown::Drawdown( const Drawdown& ot ) {}

// Destructor
Drawdown::~Drawdown() {}

vector<double> Drawdown::Calculate( Portfolio &portf_,
									Scenarios &scen_,
									Parameters p ) {
	
	m = portf_.numberOfAssets();
	n = portf_.numberOfShares();
	o = portf_.numberOfOptions();
	
	cout << "Creating Drawdown scenarios...";
	DD_Matrix_ = scen_.DDScen(portf_, p.T_, p.J, p.tSteps_);
	cout <<"Done!" <<endl;
	
	returnVec_.clear();
		// Calculates CDaR
	if ( p.CDaR_Bool_ ) {
		returnVec_.push_back(CDaR( p.CDaR_a_, p ));
	}
		// Calculates Drawdown
	if ( p.DD_Bool_ ) {
		returnVec_.push_back(DD( p ));
	}
		// Calculates Average Drawdown
	if ( p.ADD_Bool_ ) {
		returnVec_.push_back(CDaR( 0, p ));
	}
		// Calculates Maximum Drawdown
	if ( p.MDD_Bool_ ) {
		returnVec_.push_back(CDaR( 1, p ));
	}
 
return returnVec_;

} // Calculate




//---------------------------------------------------------------------------------

	// Calculates average of Drawdowns DD( CDa_=0  ) 
double Drawdown::DD( Parameters p ) {
	 // In case of single event.
	DD_Vec_.clear();
	DD_Vec_.resize(p.J,0);
	for (  j = 0; j < p.J; j++) {
		DD_Vec_[j] = DD_Matrix_(j,0) - DD_Matrix_(j,p.tSteps_);
		DD_Vec_[j] *= ( DD_Vec_[j] > 0 );
	}
	// CDaR or DD(CDa_=0) over scenarios.
	sort( DD_Vec_.begin(), DD_Vec_.end() );
	temp_ = 0;
	for ( j = 0; j < p.J; j++ ) {
		temp_ += DD_Vec_[p.J-1-j];
	}
	return temp_/p.J;
}

//---------------------------------------------------------------------------------

	// Calculates CDaR(alph=CDaR_a_), ADD(alph_=0) and MDD(alph_=1) Drawdown.
double Drawdown::CDaR ( double alph_, Parameters p ) {
	i = int(ceil( p.tSteps_*(1-alph_) ));
	if ( i == 0 ) { i = 1; } // In Case of MDD
	DD_Vec_.clear();
	DD_Vec_.resize(p.J,0);
	for (  j = 0; j < p.J; j++) {
		tempVec_.clear();
		tempVec_.resize(p.tSteps_+1,0);
		temp_ = -1234567890;
		// Counts drawdowns along paths.
		for ( k = 0; k < p.tSteps_+1; k++ ) {
			if ( DD_Matrix_(j,k) >= temp_ ) { temp_ = DD_Matrix_(j,k); }
			else { tempVec_[k] = temp_- DD_Matrix_(j,k); }
		}
		// Find the i largest values.
		sort( tempVec_.begin(), tempVec_.end() );
		temp_ = 0;
		for ( k = 0; k < i; k++ ) {
			temp_ += tempVec_[ p.tSteps_-k ];
		}
	DD_Vec_[j] = temp_/i;
	}
	// Average over scenarios
	temp_ =  0;
	for ( j = 0; j < p.J; j++ ) {
		temp_ += DD_Vec_[j];
	}
	return temp_/p.J;
}// CDaR

