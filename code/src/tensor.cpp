#include "tensor.hpp"

namespace tensor {

// Constructor and Destructor
template <typename T>
Tensor<T>::Tensor(tensor::System<T> & system) : size(system.size), dim(system.dim){
	this->setup(system);
};

template <typename T>
Tensor<T>::~Tensor(){
};


template <typename T>
void Tensor<T>::setup(tensor::System<T> & system){
	this->system = system;
};

template <typename T>
void Tensor<T>::set(std::vector<T> & theta){

	int size = this->size;
	int dim = this->dim;

	int N = this->system.N;
	int D = this->system.D;
	int n = this->system.n;

	// Tensor<T>::Type data(size,size);
	// T value = 0;
	
	// for (unsigned int i=0; i<(pow(size,dim)); i++){
	// 	data(i) =  value;
	// };
	// this->data = data;

	this->rand(theta);
};



template <typename T>
void Tensor<T>::rand(std::vector<T> & theta){

	int size = this->size;
	int dim = this->dim;

	int N = this->system.N;
	int D = this->system.D;
	int n = this->system.n;

	typedef T U;
	
	// Tensor<U>::Type data = Tensor<U>::Type::Random(size,size);

	std::random_device device;
	std::mt19937 seed(device());  
	std::uniform_real_distribution<U> distribution(-1.0, 1.0);
	Tensor<U>::Type data = Type::NullaryExpr(size,size,[&](){return distribution(seed);});

	utils::cast<U,T>(data,this->data);

	return;
};

template <typename T>
void Tensor<T>::eig(){
	
	std::vector<T> eigenvalues;
	Tensor<T>::Type eigenvectors;

	this->eig(eigenvalues,eigenvectors);

	return;
};

template <typename T>
void Tensor<T>::eig(std::vector<T> & eigenvalues, Tensor<T>::Type & eigenvectors){
	this->solver.compute(this->data);
	std::cout << this->solver.eigenvalues() << std::endl;
	// std:: cout << this->solver << std:endl;//.compute(this->data);	
	return;
};


// template <typename T>
// void Tensor<T>::convert(std::vector<std::vector<T>> & data){
	
// 	const int n = this->data.rows();
// 	const int m = this->data.cols();
// 	data.clear();
// 	data.resize(n);

// 	Eigen::Map(&data[0], n) = data;
// 	return;
// };


template <typename T>
void Tensor<T>::print(){
	std::cout << "N = " << this->system.N << std::endl;
	std::cout << "D = " << this->system.D << std::endl;
	std::cout << "d = " << this->system.d << std::endl;
	std::cout << "n = " << this->system.n << std::endl;
	std::cout << "K = " << this->system.K << std::endl;
	std::cout << "|O| = " << utils::norm<T>(this->data,this->data) << std::endl;
	std::cout << "data = \n" << this->data << std::endl;
	std::cout << std::endl;	
};

template <typename T>
void Tensor<T>::dump(std::string path){


	// Data
	io::io<T> io_data;
    io_data.write(path,this->system.data,this->data);

    // Metadata
	// io::io<int> io_metadata;

	// std::vector<std::string> header(this->system.strings);
	// std::vector<int> values = {this->system.N,this->system.D,this->system.d,this->system.n,this->system.K};

 //    io_metadata.write(path,this->system.metadata,header,values);

};

template <typename T>
void Tensor<T>::load(std::string path){
	
	io::io<T> io;

	std::vector<std::vector<T>> file;

	io.read(path,file);

	int size = this->system.size;
    
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data(size,size);

    for (int i = 0; i < size; i++){
    	for (int j = 0; j < size; j++){
        	data(i,j) = file[i][j];
    	};
    };

    this->data = data;

};

template class Tensor<double>;
template class System<double>;
template class Observables<double>;

};

