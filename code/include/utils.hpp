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
#include <variant>

#define EIGEN_USE_MKL_ALL   
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace utils {

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
Eigen::Vector<T,Eigen::Dynamic> Vector(std::vector<T> & vector);

template <typename T, std::size_t... indices>
void _assign(T & tuple, std::map<std::string,T> & map, std::vector<std::string> & keys, std::index_sequence<indices...>);

template <typename T, std::size_t size = std::tuple_size<T>::value>
void assign(T & tuple, std::map<std::string,T> & map, std::vector<std::string> & keys);

unsigned int bit(unsigned int x, unsigned int j);
unsigned int phase(unsigned int x, unsigned int j);
unsigned int flip(unsigned int x, unsigned int j);
unsigned int phaseflip(unsigned int x, unsigned int j);
unsigned int swap(unsigned int x, unsigned int i, unsigned int j);
unsigned int bitcount(unsigned int x);
};

#endif
