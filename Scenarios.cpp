// Member function definitions for Scenarios class

#include "Scenarios.hpp"

// Constructor
Scenarios::Scenarios(){ endValInitiated_ = 0; }

// Copy-constructor
Scenarios::Scenarios(const Scenarios& other){
	endValInitiated_ = other.endValInitiated_;
	m = other.m;
	n = other.n;
	o = other.o;
	shareIndex = other.shareIndex;
	r_ = other.r_;
	val_ = other.val_;
	scenarioMatrix_ = other.scenarioMatrix_;
	
	randGen_ = other.randGen_;
	tVec_ = other.tVec_;
	tMatrixVector_ = other.tMatrixVector_;
}

// Destructor
Scenarios::~Scenarios(){
	// Free memory
while( tMatrixVector_.size() > 0 ) {
	mTmp_ = tMatrixVector_[ 0 ];
	tMatrixVector_.erase( tMatrixVector_.begin() );
	delete mTmp_;
}
}

	// 
void Scenarios::clearScenarios() { endValInitiated_ = 0; }

// Creates scenarios for CVaR simulations in file scenarios.dat
Matrix<double> Scenarios::CVaRScen(Portfolio &portf_, double T, int J) {

if ( !endValInitiated_ ) {
	
	dRate_ = portf_.getRiskFreeRate();
	m = portf_.numberOfAssets();
	n = portf_.numberOfShares();
	o = portf_.numberOfOptions();
	scenarioMatrix_.resize( J, m+n+o );//  Scenario Matrix
	randMatrix_.resize( J, m+n+o );
    size = n*J;
    
	     // Get seed From File
    ifstream inFromSeed( "input/seed.txt" );
    inFromSeed >> seed;
    inFromSeed.close();
	
	if( dsfmt_get_min_array_size() >= size ) {
    	size = dsfmt_get_min_array_size();
    }
    double randArray_[size];    
    dsfmt_init_gen_rand(&randGen_, seed);    dsfmt_fill_array_open_open(&randGen_, randArray_, size);
	
		// Write new seed to file
	ofstream outToSeed( "input/seed.txt" , ios::out | ios::trunc );
	seed = int( 100000*dsfmt_genrand_open_open(&randGen_) );
    outToSeed << seed;
    outToSeed.close();
    k=0;
    for ( i = 0 ; i < n ; i++ ) {
	    for ( j = 0 ; j < J ; j++ ) {
	        randMatrix_(j,m+i) = normsinv(randArray_[k++]);
	    }
	}
    endValInitiated_ = 1;

//---------------------------------------------------------------------------------

	    // Creates scenarios
	
	// Calculate Values for Assets
	for ( i = 0 ; i < m ; i++ ) {
	    (portf_.getAsset(i)).Value() = ((portf_.getAsset(i)).InitialValue())*
	        (exp( (portf_.getAsset(i)).Rate() *T )) ;
	    for( j = 0 ; j < J ; j++ ) {
	        scenarioMatrix_(j,i) = ( portf_.getAsset(i) ).Value();
	    }
	}
	
	// Calculate Values Of Shares with given random numbers.
	for ( i = 0 ; i < n ; i++ ) {
	    Share& sto = portf_.getShare(i);
	    for ( j = 0 ; j < J ; j++ ) {
	        sto.simValue( T,randMatrix_(j,m+i) );
	        scenarioMatrix_(j,m+i) = sto.Value();
	    }
	}
	// Calculate Values Of European Options
	for ( i = 0 ; i < o ; i++ ) {
	    // Find Index Of Underlying Share
	    // Find Underlying
	    Option& opt = portf_.getOption(i);
	    Share& sto = opt.Underlying();
	    // Get Index Of Underlying (In Share Vector of Portfolio)
	    int shareIndex = 0;
	    while( shareIndex < n && 
	        sto.getName() != (portf_.getShare(shareIndex)).getName())
	            { shareIndex++; }
	        // Sets Value Of Option at time T
	    for ( j = 0 ; j < J ; j++ ) {
	    	(opt.Underlying()).Value() = scenarioMatrix_(j,m+shareIndex);
	    	opt.calcValue( T, dRate_ );
	    	
	        /*if ( opt.getType()== "CALL" ) { opt.Value() = 
	                ( max( 0.0,scenarioMatrix_(j,m+shareIndex) -opt.Strike() ) );
	        }
	        if ( opt.getType()== "PUT" ) { opt.Value() = 
	                ( max( 0.0,opt.Strike() -scenarioMatrix_(j,m+shareIndex) ) );
	        }*/
	        scenarioMatrix_(j,m+n+i) = opt.Value();
	    }
	}
	
	// Discounting of Assets.
	for ( i = 0 ; i < m+n+o ; i++ ) {
	    for ( j = 0 ; j < J ; j++ ) {
	        scenarioMatrix_(j,i) = exp(-dRate_*T)*scenarioMatrix_(j,i);
	    }
	}
	
//---------------------------------------------------------------------------------
	
/*	    // Read scenarios to file scenarios.txt
	ofstream outToFile( "input/endScenarios.txt" , ios::out |ios::trunc );
	
	for( j = 0 ; j < J ; j++ ) {
	    for ( i = 0; i < m+n+o ; i++ ) {
	        outToFile << " " << scenarioMatrix_(j,i);
	    }
	    outToFile << endl;
	}
	outToFile.close();
*/

}//Initial If-statement

return scenarioMatrix_;

}//CVaRScen








//---------------------------------------------------------------------------------

Matrix<double> Scenarios::DDScen(	Portfolio& portf_,
									double T,
									int J,
									int tSteps_ ) {
if ( !DDValInitiated_ ) {
	dRate_ = portf_.getRiskFreeRate();
	m = portf_.numberOfAssets();
	n = portf_.numberOfShares();
	o = portf_.numberOfOptions();
		
	tVec_.resize( tSteps_ +1 );
	tVec_[0] = 0.0;
	for ( i = 1 ; i < tSteps_+1 ; i++ ){
	    tVec_[i] = i*(T/tSteps_);
	}
	
		//  Scenario Matrix "tMatrixVector_"
	// Vector Indexes [k] are timesteps.
	// Matrix Rows ( ,x )are simulations
	// Matrix Columns ( x, ) are different Assets.
	
	for ( k = 0 ; k < tSteps_+1 ; k++ ){
		mTmp_ = new Matrix<double>( J, m+n+o );
	    tMatrixVector_.push_back( mTmp_ );
	}

//---------------------------------------------------------------------------------
	
		// Random number generation
	// Create random numbers for shares
	size = n*J;
    if( dsfmt_get_min_array_size() >= size ) {
    	size = dsfmt_get_min_array_size();
    }	double randArray_[size];
    
        	// Get seed From File
	ifstream inFromSeed( "input/seed.txt" );
	inFromSeed >> seed;
	inFromSeed.close();
	
    dsfmt_init_gen_rand(&randGen_, seed);	
	
		// Write new seed to file
	ofstream outToSeed( "input/seed.txt" , ios::out | ios::trunc );
	seed = int( 100000*dsfmt_genrand_open_open(&randGen_) );
    outToSeed << seed;
    outToSeed.close();

		// Brownian bridge:
	// Fills terminalvalues and zeros at beginning of Br.bridge.
	if ( endValInitiated_ ) {
		*tMatrixVector_[tSteps_] = randMatrix_;
		for ( j = 0 ; j < J ; j++ ) {
		    for ( i = m ; i < m+n ; i++ ) {
		    	(*tMatrixVector_[0])(j,i) = 0.0;
		    }
		}
	}
	else {
		dsfmt_fill_array_open_open(&randGen_, randArray_, size);
		l = 0;
		for ( j = 0 ; j < J ; j++ ) {
		    for ( i = m ; i < m+n ; i++ ) {
		        (*tMatrixVector_[tSteps_])(j,i) =		// Stratification
		            normsinv((j+randArray_[l++])/J) *sqrt(T) ;
		        (*tMatrixVector_[0])(j,i) = 0.0;
		    }
		}
	}
	
	// Construction Of Brownian Bridge
	aVec_.resize(tSteps_);
	bVec_.resize(tSteps_);
	cVec_.resize(tSteps_);
	for ( k = 1 ; k < tSteps_ ; k++ ){
	    aVec_[k] = (tVec_[tSteps_]-tVec_[k]) / (tVec_[tSteps_]-tVec_[k-1]);
	    bVec_[k] = (tVec_[k]-tVec_[k-1]) / (tVec_[tSteps_]-tVec_[k-1]);
	    cVec_[k] = sqrt( (tVec_[tSteps_]-tVec_[k]) * (tVec_[k]-tVec_[k-1])
	    	/ (tVec_[tSteps_]-tVec_[k-1]) );
	}

	for ( k = 1 ; k < tSteps_ ; k++ ){
		dsfmt_fill_array_open_open(&randGen_, randArray_, size);
		l = 0;
	    for ( j = 0 ; j < J ; j++ ) {
	        for ( i = m ; i < m+n ; i++ ) {
	        	(*tMatrixVector_[k])(j,i) = aVec_[k]*(*tMatrixVector_[k-1])(j,i)
                                      + bVec_[k]*(*tMatrixVector_[tSteps_])(j,i)
                                      + cVec_[k]*normsinv(randArray_[l++]);
	        }
		}
	}

//---------------------------------------------------------------------------------

	    // Creates scenarios for Assets/Shares/Options
	// Calculate Values for Assets
	for ( i = 0 ; i < m ; i++ ) {
		j = 0;
		(*tMatrixVector_[0])(j,i) = ((portf_.getAsset(i)).InitialValue());
		r_ = (portf_.getAsset(i)).Rate();
		for( j = 0 ; j < J ; j++ ) {
			k = 0;
			(*tMatrixVector_[k])(j,i) = (*tMatrixVector_[k])(0,i);
			for( k = 1 ; k < tSteps_+1 ; k++ ) {
				(*tMatrixVector_[k])(j,i) = (*tMatrixVector_[0])(j,i)*
				exp( tVec_[k]*r_ );
	        }
	    }
	}
	
	// Calculate Values Of Shares with given random numbers.
	for ( i = 0 ; i < n ; i++ ) {
	    Share& sto = portf_.getShare(i);
	    for ( j = 0 ; j < J ; j++ ) {
	    	for( k = 0 ; k < tSteps_+1 ; k++ ) {
	    		sto.simValue( tVec_[k], (*tMatrixVector_[k])(j,m+i) );
	        	(*tMatrixVector_[k])(j,m+i) = sto.Value();
	    	}
	    }
	}
	
	// Calculate Values Of European Options
	for ( i = 0 ; i < o ; i++ ) {
	    Option& opt = portf_.getOption(i);
	    Share& sto = opt.Underlying();
	    // Get Index Of Underlying (In Share Vector of Portfolio)
	    int shareIndex = 0;
	    while( shareIndex < n && 
	        sto.getName() != (portf_.getShare(shareIndex)).getName())
	            { shareIndex++; }
		for ( j = 0 ; j < J ; j++ ) {
			for ( k = 0 ; k < tSteps_+1 ; k++ ) {
				// Sets Value Of Option Depending on time and value of underlying
				(opt.Underlying()).Value() = (*tMatrixVector_[k])(j,m+shareIndex);
				opt.calcValue( tVec_[k], dRate_ );
	    	
	        /*if ( opt.getType()== "CALL" ) { opt.Value() = 
	                ( max( 0.0,scenarioMatrix_(j,m+shareIndex) -opt.Strike() ) );
	        }
	        if ( opt.getType()== "PUT" ) { opt.Value() = 
	                ( max( 0.0,opt.Strike() -scenarioMatrix_(j,m+shareIndex) ) );
	        }*/
				
				/*if ( T-tVec_[k] > 0 ) {
					opt.Maturity() =  T-tVec_[k];
		        	(opt.Underlying()).InitialValue() =
		        		(*tMatrixVector_[k])(j,m+shareIndex);
		        	opt.calcValue(portf_.getRiskFreeRate());
		        }
		        else {
		        	// Sets Value Of Option at terminal time
		        	opt.Maturity() = T-tVec_[k];
		        	if ( opt.getType()== "CALL" ) { opt.Value() =
		        		(max(0.0,(*tMatrixVector_[k])(j,m+shareIndex)-opt.Strike()));
		        	}
		        	if ( opt.getType()== "PUT" ) { opt.Value() =
		        		(max(0.0,opt.Strike()-(*tMatrixVector_[k])(j,m+shareIndex)));
					}
		        }*/
				(*tMatrixVector_[k])(j,m+n+i) = opt.Value();
	    	}
	    }
	}

	// Discounting
	for ( i = 0 ; i < m+n+o ; i++ ) {
	    for ( j = 0 ; j < J ; j++ ) {
	    	for ( k = 0 ; k < tSteps_ +1 ; k++ ) {
	    		(*tMatrixVector_[k])(j,i) = exp(-dRate_*tVec_[k])*
	    			(*tMatrixVector_[k])(j,i);
	    	}
	    }
	}
	DDValInitiated_ = 1;
} // If

//---------------------------------------------------------------------------------

	// Calculates portfolio values to DD_Matrix_ every time.
// Each row represents a simulation.
// Each column represents a timestep.
DD_Matrix_.resize(J, tSteps_+1);
for ( k = 0; k < tSteps_+1; k++ ) {
	for ( j = 0; j < J; j++ ) {
		DD_Matrix_(j,k) = 0;
	}
}
for ( k = 0; k < tSteps_+1; k++ ) {
	for ( j = 0; j < J; j++ ) {
		for (i = 0; i < m; i++ ){
			DD_Matrix_(j,k) += (*tMatrixVector_[k])(j,i)*
								portf_.getAssetPosition(i);
		}
		for (i = 0; i < n; i++ ){
			DD_Matrix_(j,k) += (*tMatrixVector_[k])(j,m+i)*
								portf_.getSharePosition(i);
		}
		for (i = 0; i < o; i++ ){
			DD_Matrix_(j,k) += (*tMatrixVector_[k])(j,m+n+i)*
								portf_.getOptionPosition(i);
		}
	}
}

//---------------------------------------------------------------------------------
		
/*    // Read scenarios to file scenarios.txt
	ofstream outToFile( "input/timeSeriesScenarios.txt" , ios::out |ios::trunc );
	
	for ( k = 0 ; k < tSteps_+1 ; k++ ) {
		outToFile << " Time = " << tVec_[k] << endl << endl;
		for ( j = 0 ; j < J ; j++ ) {
		    for ( i = 0; i < m+n+o ; i++ ) {
		        outToFile << " " << tMatrixVector_[k](j,i);
		    }
		    outToFile << endl << endl;
		}
	}
	outToFile.close();
*/


return DD_Matrix_;

}//DDScen

