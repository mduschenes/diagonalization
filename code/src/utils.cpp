#include "utils.hpp"

namespace utils {

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b){
	return (a.transpose()*b).trace();
};


template<typename T>
void check(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a){
	a.array() += 0;
	return;
}

template<typename T>
void check(Eigen::Vector<T, Eigen::Dynamic> & a){
	a.array() += 0;
	return;
}


template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other){
	
	other = data.template cast<U>();	

	return;
};


bool ends_with(std::string const & value, std::string const & pattern){
    if (pattern.size() > value.size()) return false;
    return std::equal(pattern.rbegin(), pattern.rend(), value.rbegin());
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
};



int spin(unsigned int x, unsigned int j){
	return 2*((x >> j) & 1) - 1;
};

int spinphase(unsigned int x, unsigned int j){
	return spin(x,j)*x;
};

int spinflip(unsigned int x, unsigned int j){
	return x ^ (1 << j);
};

int spinphaseflip(unsigned int x, unsigned int j){
	return spin(x,j) * flip(x,j);
};

int spincount(unsigned int x,unsigned int n){
	return 2*__builtin_popcount(x) - n;
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






template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & b);
template float norm<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & b);
template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & b);

template void check<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a);
template void check<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a);
template void check<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & a);

template void check<std::complex<double>>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & a);
template void check<std::complex<float>>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & a);
template void check<std::complex<int>>(Eigen::Matrix<std::complex<int>, Eigen::Dynamic, Eigen::Dynamic> & a);

template void check<double>(Eigen::Vector<double, Eigen::Dynamic> & a);
template void check<float>(Eigen::Vector<float, Eigen::Dynamic> & a);
template void check<int>(Eigen::Vector<int, Eigen::Dynamic> & a);

template void check<std::complex<double>>(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & a);
template void check<std::complex<float>>(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & a);
template void check<std::complex<int>>(Eigen::Vector<std::complex<int>, Eigen::Dynamic> & a);

template void cast<double,double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<double,float>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,double>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,double>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,float>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);

};
