// Member function definitions for Scenarios class

#include "WriteToFile.hpp"

// Constructor
writeToFile::writeToFile(){}

// Copy-constructor
writeToFile::writeToFile( const writeToFile& other ){}

// Destructor
writeToFile::~writeToFile(){}

void writeToFile::Lp( Parameters &p,
                    Portfolio &portf_,
                    Matrix<double> &scenarioMatrix_,
                    vector<double> &expectedValue_ ){

    m = portf_.numberOfAssets();
    n = portf_.numberOfShares();
    o = portf_.numberOfOptions();
    sum = 0; // variable used in matrix-building
    
    
        // Read maxproblem and restrictions to file model.lp
    ofstream outToFile( "input/CVaR_model.lp" , ios::out | ios::trunc );
    
    // Min-problem  (20)
    outToFile << "min: ";
    for ( i = 0; i < m+n+o ; i++ ) {
        outToFile << " " << -expectedValue_[i] << " x" << i;
    }
    outToFile << ";" << endl;
    
    // CVaR Constraints (21)-(22)
    outToFile << "VaR";
    for ( j = 0; j < p.J ; j++ ) {
        outToFile <<" " << p.scenProbVec_[j]/(1.0-p.CVaR_ConfLev_) <<" z" <<j ;
    }
    for ( i = 0; i < m ; i++ ) {
        sum += (portf_.getAsset(i)).InitialValue()*
                    (portf_.getInitialAssetPosition(i)) ;
    }
    for ( i = 0; i < n ; i++ ) {
        sum += (portf_.getShare(i)).InitialValue()*
                    portf_.getInitialSharePosition(i) ;
    }
    for ( i = 0; i < o ; i++ ) {
        sum += (portf_.getOption(i)).InitialValue()*
                    portf_.getInitialOptionPosition(i) ;
    }
    outToFile << " <= "; outToFile << p.omega_*sum << ";" << endl ;
    
    for ( j = 0; j < p.J; j++ ) {
        outToFile << "VaR" ;
        for ( i = 0; i < m+n+o ; i++ ) {
            outToFile << " " << scenarioMatrix_(j,i) << " x" << i ;
        }
        outToFile << " z" << j << " >= " << sum << ";" << endl ;
    }
    /*for ( j = 0; j < J; j++ ) {
        outToFile <<"z" <<j <<" >= 0;" << endl;
    }*/
    
    
    // "Percentage Of Portfolio" - Restrictions on Assets (23)
    for ( i = 0 ; i < m ; i++ ) {
        outToFile << (portf_.getAsset(i)).InitialValue()
                  << " x" << i << " <=";
        for ( j = 0 ; j < m ; j++ ) {
            outToFile << " "
                      << p.nuVec_[i]*(portf_.getAsset(j)).InitialValue() 
                      << " x" << j;
        }
        for ( j = 0 ; j < n ; j++ ) {
            outToFile << " "
                      << p.nuVec_[i]*(portf_.getShare(j)).InitialValue() 
                      << " x" << m+j;
        }
        for ( j = 0 ; j < o ; j++ ) {
            outToFile << " "
                      << p.nuVec_[i]*(portf_.getOption(j)).InitialValue() 
                      << " x" << m+n+j;
        }
        outToFile << ";" << endl;
    }
    for ( i = 0 ; i < n ; i++ ) {
        outToFile << (portf_.getShare(i)).InitialValue()
                  << " x" << m+i << " <=";
        for ( j = 0 ; j < m ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+i]*(portf_.getAsset(j)).InitialValue() 
                      << " x" << j;
        }
        for ( j = 0 ; j < n ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+i]*(portf_.getShare(j)).InitialValue() 
                      << " x" << m+j;
        }
        for ( j = 0 ; j < o ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+i]*(portf_.getOption(j)).InitialValue() 
                      << " x" << m+n+j;
        }
        outToFile << ";" << endl;
    }
    for ( i = 0 ; i < o ; i++ ) {
        outToFile << (portf_.getOption(i)).InitialValue()
                  << " x" << m+n+i << " <=";
        for ( j = 0 ; j < m ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+n+i]*(portf_.getAsset(j)).InitialValue() 
                      << " x" << j;
        }
        for ( j = 0 ; j < n ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+n+i]*(portf_.getShare(j)).InitialValue() 
                      << " x" << m+j;
        }
        for ( j = 0 ; j < o ; j++ ) {
            outToFile << " "
                      << p.nuVec_[m+n+i]*(portf_.getOption(j)).InitialValue() 
                      << " x" << m+n+j;
        }
        outToFile << ";" << endl;
    }
    
    
    // Transaction costs    (24)
    for ( i = 0; i < m ; i++ ) {
        outToFile << (portf_.getAsset(i)).InitialValue()
                  << " x" << i << " ";
    }
    for ( i = 0; i < n ; i++ ) {
        outToFile << (portf_.getShare(i)).InitialValue()
                  << " x" << m+i << " ";
    }
    for ( i = 0; i < o ; i++ ) {
        outToFile << (portf_.getOption(i)).InitialValue()
                  << " x" << m+n+i << " ";
    }
    for ( i = 0; i < m ; i++ ) {
        outToFile <<
        p.transCostVec_[i]*(portf_.getAsset(i)).InitialValue() <<
        " x_Up" << i << " ";
    }
    for ( i = 0; i < n ; i++ ) {
        outToFile <<
        p.transCostVec_[m+i]*(portf_.getShare(i)).InitialValue() <<
        " x_Up" << m+i << " ";
    }
    for ( i = 0; i < o ; i++ ) {
        outToFile <<
        p.transCostVec_[m+n+i]*(portf_.getOption(i)).InitialValue() <<
        " x_Up" << m+n+i << " ";
    }
    for ( i = 0; i < m ; i++ ) {
        outToFile <<
        p.transCostVec_[i]*(portf_.getAsset(i)).InitialValue() <<
        " x_Down" << i  << " ";
    }
    for ( i = 0; i < n ; i++ ) {
        outToFile <<
        p.transCostVec_[m+i]*(portf_.getShare(i)).InitialValue() <<
        " x_Down" << m+i << " ";
    }
    for ( i = 0; i < o ; i++ ) {
        outToFile <<
        p.transCostVec_[m+n+i]*(portf_.getOption(i)).InitialValue() <<
        " x_Down" << m+n+i << " ";
    }
    outToFile << "= " << sum << ";" << endl ;
    
    // Position changes (25)
    for ( i = 0; i < m ; i++ ) {
        outToFile << portf_.getInitialAssetPosition(i) << " = " << "x" << i
                <<" -x_Up" <<i <<" +x_Down" <<i <<";" <<endl;
    }
    for ( i = 0; i < n ; i++ ) {
        outToFile << portf_.getInitialSharePosition(i) << " = " << "x" << m+i
                  <<" -x_Up" <<m+i <<" +x_Down" <<m+i <<";" <<endl;
    }
    for ( i = 0; i < o ; i++ ) {
        outToFile << portf_.getInitialOptionPosition(i) << " = " << "x" << m+n+i
                    <<" -x_Up" <<m+n+i <<" +x_Down" <<m+n+i <<";" <<endl;
    }
    
    // Position change bounds   (26)
    for ( i = 0; i < m+n+o ; i++ ) {
        outToFile << 0 << " <= " << "x_Up" << i <<
        " <= " << p.posChangeUpBound_[i] << ";" << endl;
    }
    for ( i = 0; i < m+n+o ; i++ ) {
        outToFile << 0 << " <= " << "x_Down" << i <<
        " <= " << p.posChangeLoBound_[i] << ";" << endl;
    }
    
    // Position bounds  (27)
    for ( i = 0; i < m+n+o ; i++ ) {
        outToFile << p.posLowerBound_[i] << " <= " << "x" << i <<
        " <= " << p.posUpperBound_[i] << ";" << endl;
    }
    
    /*
    // Makes the positions discrete.
    outToFile << "int ";
    for ( i = 0; i < m+n+o-1 ; i++ ) {
        outToFile <<"x" << i <<", ";
    }
    outToFile <<"x" <<m+n+o-1 <<";" <<endl;
    */
    outToFile.close();
}// Lp

//------------------------------------------------------------------------------

	
void writeToFile::CVaR_Output ( Matrix<double>& posMatrix_ ) {
	ofstream outToCVaR( "output/CVaR_Output.txt", ios::out |ios::trunc );
	for ( j = 0 ; j < posMatrix_.size(0) ; j++) {
		for ( i = 0 ; i < posMatrix_.size(1); i++ ) {
			outToCVaR << posMatrix_(j,i) <<" ";
		}
		outToCVaR << endl;
	}
}//Output

//------------------------------------------------------------------------------

	// Writes out data to the Efficient Frontier.
// Each row represents an omega on the efficient frontier.
// Each column represents a value of each method
void writeToFile::EF( Matrix<double>& effMatrix_, Parameters& p ) {
	ofstream outToEF( "output/EF.txt" , ios::out |ios::trunc );
	if( !outToEF ) {
	    cerr << "File could not be opened\\n";
	    exit(1);
	}
	for ( j = 0; j < p.fSteps_; j++ ) {
		outToEF << 100*p.Omega_[j] << " ";
		for ( i = 0; i < p.nOfMethods_; i++ ) {
			outToEF << effMatrix_(j,i) << " ";
		}
		outToEF << endl;
	}
	outToEF.close();
}

//------------------------------------------------------------------------------

void writeToFile::LaTex(	Portfolio& portf_,
							Matrix<double>& posMatrix_,
							Matrix<double>& effMatrix_,
							Parameters& p ) {

// Writes a simple LaTex-Table

m = portf_.numberOfAssets();
n = portf_.numberOfShares();
o = portf_.numberOfOptions();
Methods_.clear();
if ( p.CVaR_Bool_ )
	Methods_.push_back("Return \\%");
if ( p.CDaR_Bool_ )
	Methods_.push_back("CDaR \\%");
if ( p.DD_Bool_ )
	Methods_.push_back("DD \\%");
if ( p.ADD_Bool_ )
	Methods_.push_back("ADD \\%");
if ( p.MDD_Bool_ )
	Methods_.push_back("MDD \\%");


cout << "Writing to LaTex File...";

ofstream outToTable( "output/table.tex" , ios::out |ios::trunc );
outToTable << "\\begin{table}[h!t]" << endl
	<< "\\caption{Run12345}" << endl	// title of Table
	<< "\\centering" << endl			// used for centering table 
	<< "\\begin{tabular}[t]{ l";
for( i=0; i < p.Omega_.size(); i=i+2 ){
	outToTable << " c";
}
outToTable << " }" << endl
	<< "\\hline\\hline \\\\" << endl
	<< "\\multicolumn{" << p.Omega_.size()/2+1 << "}{c}{CVaR Level} \\\\" << endl;
for ( i=1; i < p.Omega_.size(); i=i+2 ){
	outToTable << " & $" << 100*p.Omega_[i] <<"\\%$";
}
outToTable << " \\\\[0.5ex]";

for ( j = 0; j < effMatrix_.size(1); j++ ){
	outToTable << endl << Methods_[j];
	for ( i=1; i < effMatrix_.size(0); i=i+2 ) {
		outToTable << " &" << double(round(effMatrix_(i,j), 1));
	}
	outToTable << "\\\\";
}

outToTable << "[0.5ex]" << endl
	<< "Asset & \\multicolumn{" << p.Omega_.size()/2
		<< "}{c}{Asset Positions} \\\\[1ex]" << endl;
for ( i = 0 ; i < m ; i++ ) {
	outToTable << (portf_.getAsset(i)).getName();
	for ( j=1; j < p.Omega_.size(); j=j+2 ){
		outToTable << " & " << double(round(posMatrix_(i,j), 1));
	}
	outToTable << "\\\\" << endl;
}
for ( i = 0 ; i < n ; i++ ) {
	outToTable << (portf_.getShare(i)).getName();
	for ( j=1; j < p.Omega_.size(); j=j+2 ){
		outToTable << " & " << double(round(posMatrix_(m+i,j), 1));
	}
	outToTable << "\\\\" << endl;
}
for ( i = 0 ; i < o ; i++ ) {
	outToTable << (portf_.getOption(i)).getName();
	for ( j=1; j < p.Omega_.size(); j=j+2 ){
		outToTable << " & " << double(round(posMatrix_(m+n+i,j), 1));
	}
	outToTable << "\\\\" << endl;
}
outToTable << "[1ex] \\hline" << endl
	<< "\\end{tabular}" << endl
	<< "\\end{table} ";

outToTable.close();
cout << "Done!" << endl;


}// Latex

// Rounds a number to the specified number of decimals
double writeToFile::round( double x, int decimals ) {
	x *= pow(10,double(decimals));
	if ( x-int(x) < 0.5 ) { x = floor(x); }
	else { x = ceil(x); }
	x *= pow(10,-double(decimals));
	return x;
}


