#ifndef _UTILS_
#define _UTILS_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>
#include <numeric>
#include <algorithm>

#define EIGEN_USE_MKL_ALL   
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace utils {

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b);

template<typename T>
T norm(Eigen::SparseMatrix<T> & a, Eigen::SparseMatrix<T> & b);

template<typename T>
void check(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, T & eps);

template<typename T>
void check(Eigen::SparseMatrix<T> & a, T & eps);

template<typename T>
void check(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & a, T & eps);

template<typename T>
void check(Eigen::SparseMatrix<std::complex<T>> & a, T & eps);

template<typename T>
void check(Eigen::Vector<T, Eigen::Dynamic> & a, T & eps);

template<typename T>
bool close(T & a, T & eps);

template<typename T>
bool close(std::complex<T> & a, T & eps);

template<typename T>
bool close(T & a, T & b, T & eps);

template<typename T>
bool close(std::complex<T> & a, std::complex<T> & b, T & eps);

template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other);

bool endswith(std::string const & value, std::string const & pattern);

template <typename T>
bool isin(std::vector<T> & a,T & value);

template < typename T>
unsigned int find(std::vector<T> & a, T & value);

template <typename T> 
std::string string(const T & obj);

template <typename T> 
T number(const std::string & obj);


template <typename T>
void argsort(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<unsigned int> & indices, std::string & sorting);

unsigned int bit(unsigned int x, unsigned int j);
unsigned int phase(unsigned int x, unsigned int j);
unsigned int flip(unsigned int x, unsigned int j);
unsigned int phaseflip(unsigned int x, unsigned int j);
unsigned int bitcount(unsigned int x);

int spin(unsigned int x, unsigned int j);
int spinphase(unsigned int x, unsigned int j);
int spinflip(unsigned int x, unsigned int j);
int spinphaseflip(unsigned int x, unsigned int j);
int spincount(unsigned int x, unsigned int n);




};

#endif
