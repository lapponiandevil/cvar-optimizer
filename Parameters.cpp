/*
Container with parameters for simulations.
*/

#include "Parameters.hpp"

Parameters::Parameters() {}

Parameters::Parameters( const Parameters& o ) {
	ADD_Bool_ = o.ADD_Bool_;
	DD_Bool_ = o.DD_Bool_;
	CDaR_Bool_ = o.CDaR_Bool_;
	CVaR_Bool_ = o.CVaR_Bool_;
	MDD_Bool_ = o.MDD_Bool_;
	
	J = o.J;
	fSteps_ = o.fSteps_;
	tSteps_ = o.tSteps_;
	nOfMethods_ = o.nOfMethods_;
	nOfAssets_ = o.nOfAssets_;
	
	CDaR_a_ = o.CDaR_a_;
	CVaR_ConfLev_ = o.CVaR_ConfLev_;
	omega_ = o.omega_;
	incr_ = o.incr_;
	omega_ = o.omega_;
	T_ = o.T_;
	daysOf_ = o.daysOf_;
	wholeYear_ = o.wholeYear_;
	
	Omega_ = o.Omega_;
	scenProbVec_ = o.scenProbVec_;
	transCostVec_ = o.transCostVec_;
	nuVec_ = o.nuVec_;
	posChangeUpBound_ = o.posChangeUpBound_;
	posChangeLoBound_ = o.posChangeLoBound_;
	posUpperBound_ = o.posUpperBound_;
	posLowerBound_ = o.posLowerBound_;
}

Parameters::~Parameters() {}

void Parameters::load(int nOfAssets_) {
	// Loads input from file mainInput.txt
ifstream input( "input/mainInput.txt" );

if( !input ) {
    cerr << "File could not be opened\n";
    exit(1);
}

while( input.good() ) {
	getline(input,line_);
	switch (line_[0]) { // switch 1
		case '/': { break; }
		case 'M': {
		    switch (line_[2]) { // switch 1.1
		        case 'A': {
		            // Checks if Average Drawdown is wanted
		            first_ = line_.find_first_of("=",0)+1;
		            last_ = line_.find_last_of(";");
		            str_ = line_.substr(first_, last_-first_);
		            if (str_ == "yes"){ ADD_Bool_ = 1; }
		            else { ADD_Bool_ = 0;}
		            break;
		        }
		        case 'C': {
		        	switch (line_[3]) { // switch 1.1.1
				        case 'D': {
				        	// Checks if CDaR is wanted
						    first_ = line_.find_first_of("=",0)+1;
						    last_ = line_.find_last_of(";");
						    str_ = line_.substr(first_, last_-first_);
						    if (str_ == "yes"){ CDaR_Bool_ = 1; }
						    else { CDaR_Bool_ = 0; }
						    break;
				        }
				        case 'V': {
				            // Checks if CVaR is wanted	
				            first_ = line_.find_first_of("=",0)+1;
				            last_ = line_.find_last_of(";");
				            str_ = line_.substr(first_, last_-first_);
				            if (str_ == "yes"){ CVaR_Bool_ = 1; }
				            else { CVaR_Bool_ = 0; }
				            break;
					    }
		        	}//switch 1.1.1
		        	break;
		        }
		        case 'D': {
		            // Checks if Drawdown is wanted
		            first_ = line_.find_first_of("=",0)+1;
		            last_ = line_.find_last_of(";");
		            str_ = line_.substr(first_, last_-first_);
		            if (str_ == "yes"){ DD_Bool_ = 1; }
		            else { DD_Bool_ = 0; }
		            break;
		        }
		        case 'M': {
		            // Checks if Maximum Drawdown is wanted
		            first_ = line_.find_first_of("=",0)+1;
		            last_ = line_.find_last_of(";");
		            str_ = line_.substr(first_, last_-first_);
		            if (str_ == "yes"){ MDD_Bool_ = 1; }
		            else { MDD_Bool_ = 0; }
		            break;
		        }
		    }//switch 1.1
		    break;
		} //case 'M_' 
		
		case 'C' : {
			switch (line_[2]) { //switch 1.2
				case 'S': {
				// Reads the number of Scenarios.
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					std::istringstream strin(str_);
					strin >> J;
					break;
				}
		        case 'F': {
		            // Reads the number of steps (for an efficient frontier).
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					std::istringstream strin(str_);
					strin >> fSteps_;
					break;
		        }
		        case 'I': {
		            // Reads the size of the Increment (for efficient frontier).
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					incr_ = strtod (&str_[0],&pEnd_);
					break;
		        }
		        case 'L': {
		        	// Reads limit of CVaR value (start for Efficient Frontier).
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					omegaStart_ = strtod (&str_[0],&pEnd_);
		            break;
		        }
		        case 'C': {
		            // Reads the value for the confidence level (alpha)
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					CVaR_ConfLev_ = strtod (&str_[0],&pEnd_);
		            break;
		        }
		        case 'T': {
		            // Reads the value for the T(e)rminal time T_
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of("/");
					str_ = line_.substr(first_, last_-first_);
					daysOf_ = strtod (&str_[0],&pEnd_);
					
					first_ = line_.find_first_of("/",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					wholeYear_ = strtod (&str_[0],&pEnd_);
		            T_ = daysOf_/wholeYear_;
		            break;
		        }
		    }//switch 1.2
		    break;
	    }//case 'C_'
	    
	    case 'D': {
			switch (line_[2]) { //switch 1.3
				case 'C': {
					// Reads the confidence level for CDaR (0,1).
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					CDaR_a_ = strtod (&str_[0],&pEnd_);
					break;      	
		        }
		        case 'T': {
		            // Reads the number of time steps for
		            // the Drawdown simulation.
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					std::istringstream strin(str_);
					strin >> tSteps_;
		            if ( tSteps_ < 4 ) {
		            	tSteps_ = 4;
		            	cout << endl << "Minimum number of time steps is 4,";
		            	cout <<	" amount changed" << endl;
					}
					break;
		        }
		    }//switch 1.3
			break;
		} // case 'D_'
		

		// Portfolio Restrictions
		case 'R': {
			switch (line_[2]) { //switch 1.4
				case 'S':{
					// Reads Scenario Probabilities
					first_ = line_.find_first_of("=",0)+1;
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_, last_-first_);
					if (str_[0] == 'e' || str_[0] == 'E') {
						for ( i = 0 ; i < J ; i++ ) {
							scenProbVec_.push_back(double(1)/double(J));
						}
					}
					else {
						pStart_ = &str_[0];
						while ( pStart_[0] != ';' ) {
							val_ = strtod (pStart_,&pEnd_);
							scenProbVec_.push_back(val_);
							pStart_ = pEnd_;
						}
					}
					break;
				}
				case 'T':{
					// Reads Transaction Costs
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						transCostVec_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( transCostVec_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							transCostVec_.push_back(transCostVec_[0]);
						}
					}
					break;	
				}
				case 'R':{
					// Percentage Of Portfolio Restrictions
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						nuVec_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( nuVec_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							nuVec_.push_back(nuVec_[0]);
						}
					}
					break;
				}
				case 'U':{
					// Position CHANGE restrictions; (number of assets)
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						posChangeUpBound_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( posChangeUpBound_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							posChangeUpBound_.push_back(posChangeUpBound_[0]);
						}
					}
					break;
				}
				case 'L':{
					// Position CHANGE restrictions; (number of assets)
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						posChangeLoBound_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( posChangeLoBound_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							posChangeLoBound_.push_back(posChangeLoBound_[0]);
						}
					}
					break;
				}
				case 'u':{
					// Actual Position Restrictions
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						posUpperBound_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( posUpperBound_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							posUpperBound_.push_back(posUpperBound_[0]);
						}
					}
					break;
				}
				case 'l':{
					// Actual Position Restrictions
					first_ = line_.find_first_of("=",0);
					last_ = line_.find_last_of(";");
					str_ = line_.substr(first_+1, last_-first_);
					pStart_ = &str_[0];
					while ( pStart_[0] != ';' ) {
						val_ = strtod (pStart_,&pEnd_);
						posLowerBound_.push_back(val_);
						pStart_ = pEnd_;
					}
					if ( posLowerBound_.size() < nOfAssets_ ) {
						for (i = 1; i < nOfAssets_; i++) {
							posLowerBound_.push_back(posLowerBound_[0]);
						}
					}
					break;
				}
				default: { break; }
			} //switch 1.4
			break;
		} // case 'R_'
		
		default: {
			input.ignore(128,'\n');
			break;
		}
	}//switch 1
} // while
input.close();


// Creates the omega-values for efficient frontier
Omega_.resize(fSteps_,0);
for( int i = 0; i < fSteps_; i++ ){
	Omega_[i] = (incr_*i) +omegaStart_;
}

nOfMethods_ =	CVaR_Bool_ +
				CDaR_Bool_ +
				DD_Bool_ +
				ADD_Bool_ +
				MDD_Bool_;


}// load();
