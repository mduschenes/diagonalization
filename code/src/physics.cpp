#include "physics.hpp"

namespace physics {

int spin(unsigned int x, unsigned int i){
	return 2*((x >> i) & 1) - 1;
};

int spinphase(unsigned int x, unsigned int i){
	return spin(x,i)*x;
};

int spinflip(unsigned int x, unsigned int i){
	return x ^ (1 << i);
};

int spinphaseflip(unsigned int x, unsigned int i){
	return spin(x,i) * spinflip(x,i);
};

int spinswap(unsigned int x, unsigned int i, unsigned int j){
	return x ^ ((1 << i) + (1 << j));
};

int spincount(unsigned int x,unsigned int n){
	return 2*__builtin_popcount(x) - n;
};

template<typename T>
T expectation(Eigen::Vector<T, Eigen::Dynamic> & data, Eigen::Vector<T, Eigen::Dynamic> & value, T & eps){
	return data.cwiseAbs2().dot(value.cwiseAbs());
};

template<typename T>
T expectation(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data,Eigen::Vector<T, Eigen::Dynamic> & value, T & eps){
	return data.cwiseAbs2().dot(value.cwiseAbs());
};

template<typename T>
T expectation(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data,Eigen::Vector<std::complex<T>, Eigen::Dynamic> & value, T & eps){
	return data.cwiseAbs2().dot(value.cwiseAbs());
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

	if (value < eps){
		value = 0;
	};
	
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

	if (value < eps){
		value = 0;
	};
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

// 	Eigen::Vector<std::complex<T>,Eigen::Dynamic>::Map(data.data(),size);
	
// 	eigenvalues = (eigenvalues.array().abs() < eps).select(1,eigenvalues);

// 	value = -eigenvalues.dot(eigenvalues.array().log().matrix());
// 	return value;
// };


// int ditphase(unsigned int & x, unsigned int & d,unsigned int & i, unsigned int & u){
// 	return (x >> i) & u;
// };

// int ditraise(unsigned int & x, unsigned int & d,unsigned int & i, unsigned int & u){
// 	return (x + 1)*((d - 1 - (x&(pow(d,i))))/d);
// };

// int ditlower(unsigned int & x, unsigned int & d,unsigned int & i, unsigned int & u){
// 	return (x - 1)*((d - 1 + (x&(pow(d,i))))/d);
// };


template double expectation(Eigen::Vector<double, Eigen::Dynamic> & data,Eigen::Vector<double, Eigen::Dynamic> & value, double & eps);
template float expectation(Eigen::Vector<float, Eigen::Dynamic> & data,Eigen::Vector<float, Eigen::Dynamic> & value, float & eps);

template double expectation(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data,Eigen::Vector<double, Eigen::Dynamic> & value, double & eps);
template float expectation(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data,Eigen::Vector<float, Eigen::Dynamic> & value, float & eps);

template double expectation(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data,Eigen::Vector<std::complex<double>, Eigen::Dynamic> & value, double & eps);
template float expectation(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data,Eigen::Vector<std::complex<float>, Eigen::Dynamic> & value, float & eps);

template double entropy(Eigen::Vector<double, Eigen::Dynamic> & data, unsigned int & n, double & eps);
template float entropy(Eigen::Vector<float, Eigen::Dynamic> & data, unsigned int & n, float & eps);

template double entropy(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data, unsigned int & n, double & eps);
template float entropy(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data, unsigned int & n, float & eps);

};
