#ifndef _UTILS_
#define _UTILS_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>

#include <Eigen/Dense>
#include <LBFGS.h>
#include "hdf5.h"


namespace utils {


template<class T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);

template<class T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);


};

#endif
