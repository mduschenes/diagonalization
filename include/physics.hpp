#ifndef _PHYSICS_
#define _PHYSICS_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <variant>


// #define EIGEN_USE_MKL_ALL
#define NUM_THREADS 7
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SVD>
#include <unsupported/Eigen/MatrixFunctions>

#include "linalg.hpp"

namespace physics {

int spin(unsigned int x, unsigned int i);
int spinphase(unsigned int x, unsigned int i);
int spinflip(unsigned int x, unsigned int i);
int spinphaseflip(unsigned int x, unsigned int i);
int spinswap(unsigned int x, unsigned int i, unsigned int j);
int spincount(unsigned int x, unsigned int n);

template<typename T>
T expectation(Eigen::Vector<T, Eigen::Dynamic> & data,Eigen::Vector<T, Eigen::Dynamic> & value, T & tol);

template<typename T>
T expectation(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data,Eigen::Vector<T, Eigen::Dynamic> & value, T & tol);

template<typename T>
T expectation(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data,Eigen::Vector<std::complex<T>, Eigen::Dynamic> & value, T & tol);

template<typename T>
T entropy(Eigen::Vector<T, Eigen::Dynamic> & data, unsigned int & n, T & tol);

template<typename T>
T entropy(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data, unsigned int & n, T & tol);

};

#endif
