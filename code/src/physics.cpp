#include "physics.hpp"

namespace physics {

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
	return spin(x,j) * spinflip(x,j);
};

int spinswap(unsigned int x, unsigned int i, unsigned int j){
	return x ^ ((1 << i) + (1 << j));
};

int spincount(unsigned int x,unsigned int n){
	return 2*__builtin_popcount(x) - n;
};

template<typename T>
T expectation(Eigen::Vector<T, Eigen::Dynamic> & data,Eigen::Vector<T, Eigen::Dynamic> & weights, T & eps){
	return weights.dot(data);
};

template<typename T>
std::complex<T> expectation(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data,Eigen::Vector<T, Eigen::Dynamic> & weights, T & eps){
	return weights.dot(data);
};


template<typename T>
T entropy(Eigen::Vector<T, Eigen::Dynamic> & data, unsigned int & n, T & eps){

	T value;
	unsigned int size = data.size();
	std::string sigma = "LM";
	Eigen::Vector<T,Eigen::Dynamic> singularvalues;

	Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> matrix = data.reshaped(n,size/n);

	linalg::svd(matrix,singularvalues,n,sigma,eps);

	singularvalues = singularvalues.array().square().matrix();

	value = -singularvalues.dot(singularvalues.array().log().matrix());

	return value;
};



template<typename T>
T entropy(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data, unsigned int & n, T & eps){

	T value;		
	unsigned int size = data.size();
	std::string sigma = "LM";
	Eigen::Vector<T,Eigen::Dynamic> singularvalues;

	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic> matrix = data.reshaped(n,size/n);

	linalg::svd(matrix,singularvalues,n,sigma,eps);

	singularvalues = singularvalues.cwiseAbs2();

	value = -singularvalues.dot(singularvalues.array().log().matrix());

	return value;
};


// template<typename T>
// T entropy(Eigen::Vector<T, Eigen::Dynamic> & data, unsigned int & n, T & eps){

// 	T value;
// 	unsigned int size = data.size();

// 	Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>::Map(data.data(),n,size/n);

// 	Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> matrix = data * data.adjoint();

// 	value = -(matrix * matrix.log()).trace();

// 	Eigen::Vector<T,Eigen::Dynamic>::Map(data.data(),size);
	
// 	return value;
// };



// template<typename T>
// T entropy(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data, unsigned int & n, T & eps){

// 	T value;
// 	unsigned int size = data.size();

// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic>::Map(data.data(),n,size/n);

// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic> matrix = data * data.adjoint();

// 	value = -(matrix * matrix.log()).trace();

// 	Eigen::Vector<std::complex<T>,Eigen::Dynamic>::Map(data.data(),size);
	
// 	return value;
// };


// template<typename T>
// T entropy(Eigen::Vector<T, Eigen::Dynamic> & data, unsigned int & n, T & eps){

// 	T value;
// 	unsigned int size = data.size();

// 	Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>::Map(data.data(),n,size/n);

// 	Eigen::SparseMatrix<T,Eigen::ColMajor> matrix = (data * data.adjoint()).sparseView();
// 	Eigen::Vector<T,Eigen::Dynamic> eigenvalues;
// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic> eigenvectors;
// 	unsigned int n = size - 1;
// 	std::string sigma = "LA";
// 	T eps = 0;

// 	linalg::eigh(matrix,eigenvalues,eigenvectors,n,sigma,eps);

// 	std::cout << n << " " << size << " " << eigenvalues << std::endl;
// 	std::cout << std::endl;

// 	Eigen::Vector<T,Eigen::Dynamic>::Map(data.data(),size);
	
// 	eigenvalues = (eigenvalues.array().abs() <= eps).select(1,eigenvalues);

// 	value = T(1) - eigenvalues.sum();
// 	value = -value*std::log(value);
// 	value += -eigenvalues.dot(eigenvalues.array().log().matrix());
// 	return value;
// };

// template<typename T>
// T entropy(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data, unsigned int & n, T & eps){
	
// 	T value;
// 	unsigned int size = data.size();

// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic>::Map(data.data(),n,size/n);

// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic> matrix = data * data.adjoint();
// 	Eigen::Vector<T,Eigen::Dynamic> eigenvalues;
// 	Eigen::Matrix<std::complex<T>,Eigen::Dynamic,Eigen::Dynamic> eigenvectors;
// 	unsigned int n = size;
// 	std::string sigma = "LA";
// 	T eps = 1e-30;

// 	linalg::eigh(matrix,eigenvalues,eigenvectors,n,sigma,eps);

// 	std::cout << n << " " << size << " " << eigenvalues << std::endl;
// 	std::cout << std::endl;

// 	Eigen::Vector<std::complex<T>,Eigen::Dynamic>::Map(data.data(),size);
	
// 	eigenvalues = (eigenvalues.array().abs() < eps).select(1,eigenvalues);

// 	value = -eigenvalues.dot(eigenvalues.array().log().matrix());
// 	return value;
// };


// int ditphase(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x >> j) & u;
// };

// int ditraise(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x + 1)*((d - 1 - (x&(pow(d,j))))/d);
// };

// int ditlower(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x - 1)*((d - 1 + (x&(pow(d,j))))/d);
// };


template double expectation(Eigen::Vector<double, Eigen::Dynamic> & data,Eigen::Vector<double, Eigen::Dynamic> & weights, double & eps);
template float expectation(Eigen::Vector<float, Eigen::Dynamic> & data,Eigen::Vector<float, Eigen::Dynamic> & weights, float & eps);

template std::complex<double> expectation(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data,Eigen::Vector<double, Eigen::Dynamic> & weights, double & eps);
template std::complex<float> expectation(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data,Eigen::Vector<float, Eigen::Dynamic> & weights, float & eps);

template double entropy(Eigen::Vector<double, Eigen::Dynamic> & data, unsigned int & n, double & eps);
template float entropy(Eigen::Vector<float, Eigen::Dynamic> & data, unsigned int & n, float & eps);

template double entropy(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data, unsigned int & n, double & eps);
template float entropy(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data, unsigned int & n, float & eps);

};
