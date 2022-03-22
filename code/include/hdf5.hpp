#ifndef _HDF5_
#define _HDF5_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector> 
#include <complex>
#include <cmath>
#include <map>

#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <H5Cpp.h>

namespace hdf5 {

template<typename T>
H5::DataType H5Type();

template<typename T>
void dump(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);

template<typename T>
void dump(std::string & path, std::string & name, Eigen::SparseMatrix<T> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<T> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<T, Eigen::Dynamic> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<T>> & data);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data);



template<typename T>
void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,T> & attributes);

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<T, Eigen::Dynamic>> & attributes);

template<typename T>
void dump(std::string & path, std::string & name, std::vector<T> & data);

};

#endif
