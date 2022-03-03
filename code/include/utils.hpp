#ifndef _UTILS_
#define _UTILS_

#include <iostream>
#include <stdlib.h>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>

#include <Eigen/Dense>
#include <LBFGS.h>



namespace utils {


Eigen::MatrixXd commutator(Eigen::MatrixXd & A, Eigen::MatrixXd & B);

double norm(Eigen::MatrixXd & A, Eigen::MatrixXd & B);



};

#endif
