#include "utils.hpp"

namespace utils {


Eigen::MatrixXd commutator(Eigen::MatrixXd & A, Eigen::MatrixXd & B){
	return A*B - B*A;
};

double norm(Eigen::MatrixXd & A, Eigen::MatrixXd & B){
	return (A.transpose()*B).trace();
};


};
