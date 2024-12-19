#ifndef MATRIX_HXX
#define MATRIX_HXX    

#include <vector>
#include <cassert>

template<typename T> class Matrix
{
private:
    int m_; // number of rows
    int n_; // number of columns  
    std::vector<T> v_; // values
public:
    Matrix() : m_(0), n_(0), v_(0) {}
    Matrix(int m, int n) : m_(m), n_(n), v_(m*n) {} 
    
    Matrix(const Matrix&A) {
        m_=A.m_; 	
	    n_=A.n_; 
	    v_.resize(A.v_.size());
	    v_=A.v_;
    }
    
    ~Matrix() {}
    
    T& operator()(const int i, const int j) {
        if(i<0||i>=m_){
            std::cout << "Matrix::operator() Index out of bounds\n";
        }
        if(j<0||j>=n_){
            std::cout << "Matrix::operator() Index out of bounds\n";
        }
    return v_.at(j*m_+i);      
    }
    
    const T& operator()(const int i, const int j) const {
        if(i<0||i>=m_){
            std::cout << "Matrix::operator() Index out of bounds\n";
        }
        if(j<0||j>=n_){
            std::cout << "Matrix::operator() Index out of bounds\n";
        }
    return v_.at(j*m_+i);
    }
    
    int size(int i) const {
	   if (i==0) return m_; 
	   if (i==1) return n_; 
	return 0;
    }
    
    Matrix& resize(const int m, const int n) {
        m_=m;
        n_=n;
        v_.resize(m*n);
    return *this;
    }
    
    Matrix& operator=(const Matrix&A) {
        m_=A.m_;
        n_=A.n_;
        v_.resize(A.v_.size());
        v_=A.v_; 
    return *this;   
    }
    
    Matrix& operator=(const T&value) {
        for (unsigned int i=0; i<v_.size(); i++)
        v_.at(i)=value;
	return *this;   
    }
    
    friend std::ostream& operator<<(std::ostream &os, const Matrix<T> &A) {
        int m=A.size(0);
        int n=A.size(1);
        for (int i=0; i<m; i++) {
            for (int j=0; j<n; j++)
	           os << A(i,j) << " " ;
            os << "\n";
        }
    return os;
    }
      
};                                                                     
#endif

