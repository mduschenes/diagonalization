#include "utils.hpp"

namespace utils {

template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return A*B - B*A;
};

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return (A.transpose()*B).trace();
};


bool ends_with(std::string const & value, std::string const & pattern){
    if (pattern.size() > value.size()) return false;
    return std::equal(pattern.rbegin(), pattern.rend(), value.rbegin());
};


template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other){
	
	other = data.template cast<U>();	

	return;
};



unsigned int bit(unsigned int x, unsigned int j){
	return (x >> j) & 1;
};

unsigned int phase(unsigned int x, unsigned int j){
	return bit(x,j)*x;
};

unsigned int flip(unsigned int x, unsigned int j){
	return x ^ (1 << j);
};

unsigned int phaseflip(unsigned int x, unsigned int j){
	return bit(x,j) * flip(x,j);
};

unsigned int bitcount(unsigned int x){
	return __builtin_popcount(x);
	// unsigned int p = 0;
	// unsigned int j = 0;
	// while((p<=m)&&(j<n)){
	// 	p += bit(x,j);
	// 	j++;
	// };
	// return p==m;
};


// int ditphase(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x >> j) & u;
// };

// int ditraise(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x + 1)*((d - 1 - (x&(pow(d,j))))/d);
// };

// int ditlower(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x - 1)*((d - 1 + (x&(pow(d,j))))/d);
// };






template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
// template float norm<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> &);
// template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> commutator<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
// template Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> commutator<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> &);
// template Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> commutator<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);


template void cast<double,double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
// template void cast<double,float>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
// template void cast<float,float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
// template void cast<float,double>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
// template void cast<int,double>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
// template void cast<int,float>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);

};
