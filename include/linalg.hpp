#ifndef _LINALG_
#define _LINALG_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <variant>

#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues> 
#include <unsupported/Eigen/ArpackSupport>
#include <unsupported/Eigen/MatrixFunctions>
#include <unsupported/Eigen/CXX11/Tensor>

#include "utils.hpp"

// #include <Spectra/SymEigsSolver.h>
// #include <Spectra/MatOp/DenseSymMatProd.h>
// #include <Spectra/MatOp/SparseSymMatProd.h>


namespace linalg {


template<typename T>
void eigh(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);


template<typename T>
void eigh(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void eigh(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);


// template<typename T>
// void eigs(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);


// template<typename T>
// void eigs(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

// template<typename T>
// void eigs(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void svd(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
void svd(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, T & eps);

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b);

template<typename T>
T norm(Eigen::SparseMatrix<T> & a, Eigen::SparseMatrix<T> & b);

template <typename T>
void permute(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);

template <typename T>
void permute(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);

template <typename T>
void permute(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices);

template <typename T>
void permute(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & a, std::vector<int> & indices);

template <typename T>
void argsort(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);

template<typename T>
void trace(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, T & eps);

template<typename T>
void trace(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, T & eps);

};

#endif
