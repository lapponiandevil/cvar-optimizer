#include <cstdlib>
#include <iostream>
#include <fstream>

#include "Asset.hpp"
#include "Stock.hpp"

using namespace std;

int main(){
	
	double val_;
	Stock sto_;
	
	sto_.Rate() = 0.08;
	sto_.InitialValue() = 1.0;
	
	ofstream outToVol( "vol.txt" , ios::out |ios::trunc );
	if( !outToVol ) {
	    cerr << "File could not be opened\n";
	    exit(1);
	}
	for ( int i = 0; i < 9; i++ ) {
		val_ = i*.1;
		sto_.Volatility() = val_;
		sto_.simValue( 1.0, 0.0 );
		cout << "M " << sto_.Rate() << endl;
		outToVol << val_ << " " << sto_.Value() << endl;
	}
	outToVol.close();
	
return(0);	
}