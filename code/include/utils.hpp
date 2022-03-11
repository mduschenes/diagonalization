#ifndef _UTILS_
#define _UTILS_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>

#include <Eigen/Dense>

namespace utils {


template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);


unsigned int bit(unsigned int x, unsigned int j);
unsigned int phase(unsigned int x, unsigned int j);
unsigned int flip(unsigned int x, unsigned int j);
unsigned int phaseflip(unsigned int x, unsigned int j);
unsigned int bitcount(unsigned int x);

bool ends_with(std::string const & value, std::string const & pattern);


template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other);


};

#endif
