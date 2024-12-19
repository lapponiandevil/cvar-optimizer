#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "CVaR.hpp"
#include "Drawdown.hpp"
#include "Scenarios.hpp"
#include "Portfolio.hpp"
#include "writeToFile.hpp"
#include "Parameters.hpp"
#include "Matrix.hpp"

using namespace std;

int main(){

int i,j,m,n,o;
CVaR cvar_;
Drawdown dd_;
Scenarios scen_;
writeToFile toFile_;
static Portfolio portf_;
portf_.load();
m = portf_.numberOfAssets();
n = portf_.numberOfShares();
o = portf_.numberOfOptions();
static Parameters p;
p.load(m+n+o);

Matrix<double>	effMatrix_( p.fSteps_, p.nOfMethods_ ),
				posMatrix_( m+n+o, p.fSteps_ );
vector<double> returnVec_( p.nOfMethods_,0);

//------------------------------------------------------------------------------

		// Run Parts Chosen in mainInput.txt.

for ( j = 0; j < p.fSteps_; j++ ) {
	cout << endl << "Step " << j+1 << "/" << p.fSteps_ << endl;
	double CVaR_;
	returnVec_.clear();
	returnVec_.resize( p.nOfMethods_, 0 );
	p.omega_ = p.Omega_[j];
	
	// CVaR Simulations
	if ( p.CVaR_Bool_ ) {
		CVaR_ = cvar_.Optimize( portf_, scen_, toFile_, p );
	}
	// Drawdown Simulations
	if ( p.ADD_Bool_ || p.MDD_Bool_ || p.CDaR_Bool_ || p.DD_Bool_ ) {
		returnVec_ = dd_.Calculate( portf_, scen_, p );
	}
	returnVec_.insert( returnVec_.begin(), CVaR_ );
	
		// Counts the relative return of the portfolio
	double val_ = 0;
	for ( i = 0; i < m ; i++ ) {
		val_ += (portf_.getAsset(i)).InitialValue()*
			(portf_.getInitialAssetPosition(i)) ;
	}
	for ( i = 0; i < n ; i++ ) {
		val_ += (portf_.getShare(i)).InitialValue()*
			portf_.getInitialSharePosition(i) ;
	}
	for ( i = 0; i < o ; i++ ) {
		val_ += (portf_.getOption(i)).InitialValue()*
			portf_.getInitialOptionPosition(i) ;
	}
	returnVec_[0] -= val_;
	for ( i = 0; i < p.nOfMethods_; i++ ) {
		// Converts R_ to Percent.
		returnVec_[i] /= val_;
		returnVec_[i] *= 100;
		effMatrix_(j,i) = returnVec_[i];
	}
	
		// Saves positions of portfolio to posMatrix_
	// Each row represents an asset.
	// Each column represents a simulation.
	for ( i = 0; i<m; i++ ){
		posMatrix_(i,j) = portf_.getAssetPosition(i);
	}
	for ( i = 0; i<n; i++ ){
		posMatrix_(m+i,j) = portf_.getSharePosition(i);
	}
	for ( i = 0; i<o; i++ ){
		posMatrix_(m+n+i,j) = portf_.getOptionPosition(i);
	}
}
scen_.clearScenarios();

// Writes positions for portfolios along the efficient frontier.
// Filename Table.tex and CVaR_Output.txt
toFile_.LaTex( portf_, posMatrix_, effMatrix_, p );
toFile_.CVaR_Output( posMatrix_ );

// Writes Efficient frontier to file EF.txt
toFile_.EF( effMatrix_, p);

cout << posMatrix_;

return(0);
}

