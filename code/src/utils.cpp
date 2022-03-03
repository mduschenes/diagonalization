#include "utils.hpp"

namespace utils {

template<class T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return A*B - B*A;
};

template<class T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return (A.transpose()*B).trace();
};


template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> commutator<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> commutator<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

};
