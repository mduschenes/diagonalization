#include "utils.hpp"

namespace utils {

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b){
	return (a.transpose()*b).trace();
};


template<typename T>
T norm(Eigen::SparseMatrix<T> & a, Eigen::SparseMatrix<T> & b){
	return 0;
};


template<typename T>
void check(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, T & eps){

	T val = 0;

	a.array() += 0;

	for (unsigned int i=0;i<a.rows();i++){
		for (unsigned int j=0;j<a.cols();j++){
			if (close(a(i,j),eps)){
				a(i,j) = val;
			};
		};
	};
	return;
};

template<typename T>
void check(Eigen::SparseMatrix<T> & a, T & eps){
	a.coeffs().array() += 0;
	return;
};

template<typename T>
void check(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & a, T & eps){
	a.array() += 0;
	return;
};

template<typename T>
void check(Eigen::SparseMatrix<std::complex<T>> & a, T & eps){
	a.coeffs().array() += 0;
	return;
};

template<typename T>
void check(Eigen::Vector<T, Eigen::Dynamic> & a, T & eps){

	T val = 0;

	a.array() += 0;
	for (unsigned int i=0;i<a.size();i++){
		if (close(a[i],eps)){
			a[i] = val;
		};
	};
	return;
};


template<typename T>
bool close(T & a, T & eps){
	return (std::abs(a) < std::abs(eps));
};


template<typename T>
bool close(std::complex<T> & a, T & eps){
	return (std::abs(a) < std::abs(eps));
};

template<typename T>
bool close(T & a, T & b, T & eps){
	return ((std::abs(a-b)/std::max((T)1,std::max(std::abs(a),std::abs(b)))) < std::abs(eps));
};

template<typename T>
bool close(std::complex<T> & a, std::complex<T> & b, T & eps){
	return ((std::abs(a-b)/std::max((T)1,std::max(std::abs(a),std::abs(b)))) < std::abs(eps));
};


template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other){
	
	other = data.template cast<U>();	

	return;
};


bool endswith(std::string const & value, std::string const & pattern){
	 if (pattern.size() > value.size()) return false;
	 return std::equal(pattern.rbegin(), pattern.rend(), value.rbegin());
};


template <typename T>
bool isin(std::vector<T> & a,T & value){
	return (std::find(a.begin(), a.end(), value) != a.end());
};

template <typename T>
unsigned int find(std::vector<T> & a, T & value){
	typename std::vector<T>::iterator iterator = std::find(a.begin(), a.end(), value);
	unsigned int index = std::distance(a.begin(),iterator);
	return index;
};


template <typename T>
void permute(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = Vector(indices);
	if (axis == 0){
		a = permuation * a;		
	}
	else if ((axis == 1) or (axis == -1)){
		a = a * permuation;		
	}
	else {
		a = permuation * a;		
	};
	return;
};

template <typename T>
void permute(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = Vector(indices);
	a = permuation * a;	
	return;
};


template <typename T> 
std::string string(const T & obj){ 
	std::ostringstream stream; 
	stream << obj; 
	std::string string = stream.str();
	return string;
};


template <typename T> 
T number(const std::string & obj){
	T number = std::stod(obj);
	return number;
};


template <typename T>
Eigen::Vector<T,Eigen::Dynamic> Vector(std::vector<T> & vector){
	T * ptr = &vector[0];
	Eigen::Map<Eigen::Vector<T,Eigen::Dynamic>> Vector(ptr, vector.size());
	return Vector;
};


template <typename T>
void argsort(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting){

	indices.resize(a.size());
	std::iota(indices.begin(),indices.end(),0);

	bool (*algorithm)(T,T);
	if (sorting == "<="){
		algorithm = [](T a, T b) {return a <= b;};
	}
	else if (sorting == "<"){
		algorithm = [](T a, T b) {return a < b;};
	}
	else if (sorting == ">="){
		algorithm = [](T a, T b) {return a >= b;};
	}
	else if (sorting == ">"){
		algorithm = [](T a, T b) {return a > b;};
	}
	else {
		algorithm = [](T a, T b) {return a <= b;};
	};

	std::stable_sort(indices.begin(), indices.end(),[&a,&algorithm](unsigned int i, unsigned int j) {return algorithm(a(i),a(j));}); 

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

template double norm<double>(Eigen::SparseMatrix<double> & a,Eigen::SparseMatrix<double> & b);
template float norm<float>(Eigen::SparseMatrix<float> & a,Eigen::SparseMatrix<float> & b);
template int norm<int>(Eigen::SparseMatrix<int> & a,Eigen::SparseMatrix<int> & b);

template void check<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a, double & eps);
template void check<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a, float & eps);

template void check<double>(Eigen::SparseMatrix<double> & a, double & eps);
template void check<float>(Eigen::SparseMatrix<float> & a, float & eps);

template void check<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & a, double & eps);
template void check<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & a, float & eps);

template void check<double>(Eigen::SparseMatrix<std::complex<double>> & a, double & eps);
template void check<float>(Eigen::SparseMatrix<std::complex<float>> & a, float & eps);

template void check<double>(Eigen::Vector<double, Eigen::Dynamic> & a, double & eps);
template void check<float>(Eigen::Vector<float, Eigen::Dynamic> & a, float & eps);

template bool close<double>(double & a, double & eps);
template bool close<float>(float & a, float & eps);
template bool close<int>(int & a, int & eps);

template bool close<double>(std::complex<double> & a, double & eps);
template bool close<float>(std::complex<float> & a, float & eps);

template bool close<double>(double & a, double & b, double & eps);
template bool close<float>(float & a, float & b, float & eps);
template bool close<int>(int & a, int & b, int & eps);
template bool close<double>(std::complex<double> & a, std::complex<double> & b, double & eps);
template bool close<float>(std::complex<float> & a, std::complex<float> & b, float & eps);


template bool isin<double>(std::vector<double> & a,double & value);
template bool isin<float>(std::vector<float> & a,float & value);
template bool isin<int>(std::vector<int> & a,int & value);
template bool isin<unsigned int>(std::vector<unsigned int> & a,unsigned int & value);

template unsigned int find<double>(std::vector<double> & a,double & value);
template unsigned int find<float>(std::vector<float> & a,float & value);
template unsigned int find<int>(std::vector<int> & a,int & value);
template unsigned int find<unsigned int>(std::vector<unsigned int> & a,unsigned int & value);


template void permute<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<unsigned int>(Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);

template void permute<double>(Eigen::Vector<double, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<float>(Eigen::Vector<float, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<int>(Eigen::Vector<int, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<unsigned int>(Eigen::Vector<unsigned int, Eigen::Dynamic> & a, std::vector<int> & indices);


template std::string string(const double & obj);
template std::string string(const float & obj);
template std::string string(const int & obj);
template std::string string(const std::string & obj);

template double number<double>(const std::string & obj);
template float number<float>(const std::string & obj);
template int number<int>(const std::string & obj);

template Eigen::Vector<double,Eigen::Dynamic> Vector<double>(std::vector<double> & vector);
template Eigen::Vector<float,Eigen::Dynamic> Vector<float>(std::vector<float> & vector);
template Eigen::Vector<int,Eigen::Dynamic> Vector<int>(std::vector<int> & vector);
template Eigen::Vector<unsigned int,Eigen::Dynamic> Vector<unsigned int>(std::vector<unsigned int> & vector);

template void argsort<double>(Eigen::Vector<double, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);
template void argsort<float>(Eigen::Vector<float, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);
template void argsort<int>(Eigen::Vector<int, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);

template void cast<double,double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<double,float>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,double>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,double>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,float>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);

};
