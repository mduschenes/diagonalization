#ifndef _UTILS_
#define _UTILS_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>

#define EIGEN_USE_MKL_ALL   
#include <Eigen/Dense>

namespace utils {

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b);

template<typename T>
void check(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a);

template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other);

bool ends_with(std::string const & value, std::string const & pattern);


unsigned int bit(unsigned int x, unsigned int j);
unsigned int phase(unsigned int x, unsigned int j);
unsigned int flip(unsigned int x, unsigned int j);
unsigned int phaseflip(unsigned int x, unsigned int j);
unsigned int bitcount(unsigned int x);

int spin(unsigned int x, unsigned int j);
int spinphase(unsigned int x, unsigned int j);
int spinflip(unsigned int x, unsigned int j);
int spinphaseflip(unsigned int x, unsigned int j);
int spincount(unsigned int x);




};

#endif
