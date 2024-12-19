/*
Container with parameters for simulations.
*/

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Parameters {
		
public:
	Parameters();
	Parameters( const Parameters& );
	~Parameters();
	void load( int );

	bool 	ADD_Bool_,
			DD_Bool_,
			CDaR_Bool_,
			CVaR_Bool_,
			MDD_Bool_;
			
	char	*pStart_, *pEnd_;
	
	string	line_, str_;
	
	int 	first_, last_,	// Used in extraction of parameters
			J,				// Number of Scenarios
			fSteps_,		// Number of timesteps in efficient frontier.
			tSteps_,		// Number of timesteps in simulation of a Drawdown.
			nOfMethods_,	// Number of methods used.
			nOfAssets_,		// Total number of derivatives
			i;
			
	double	CDaR_a_,		// Confidence Level for CVaR-calculations.
			CVaR_ConfLev_,	// Confidence Level for CVaR-calculations.
			omega_,			// Allowed CVaR-Level, a percentage of the value of
							// portfolio. Used in calculations of each step.
			omegaStart_,	// Startvalue for efficient frontier.
			incr_,			// Size of increment in efficient frontier (optional).
			T_,				// Terminal time  "days of / whole year"
			daysOf_,wholeYear_,
			val_;
	
	vector<double>	Omega_,			// Used for creation of Efficient Frontier
					scenProbVec_,				// Probability of scenarios
					transCostVec_,		// Transaction cost for assets
					nuVec_,						// Maximum part, with which
												// an asset can contribute
												// to the portfolio.
					posChangeUpBound_,			// Position change restriction,
					posChangeLoBound_,			// both upper and lower
					posUpperBound_,				// Actual position restrictions,
					posLowerBound_;				// upper and lower.

};

#endif
