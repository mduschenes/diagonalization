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
#include <H5Cpp.h>


namespace utils {


template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B);


bool ends_with(std::string const & value, std::string const & pattern);


template<typename T>
H5::DataType H5Type();


template<typename T>
void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);

template<typename T>
void hdf5_to_eigen(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);

template<typename T>
void vector_to_hdf5(std::string & path, std::string & name, std::vector<T> & data);

template<typename T>
void hdf5_to_vector(std::string & path, std::string & name, std::vector<T> & data);

};

#endif
