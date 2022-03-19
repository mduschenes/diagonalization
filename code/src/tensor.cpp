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
void Tensor<T>::rand(){

	// int size = this->size;
	// int dim = this->dim;

	// int N = this->system.N;
	// int D = this->system.D;
	// int n = this->system.n;
	// std::vector<T> parameters = this->system.parameters;
	
	// typedef T U;

	// std::random_device device;
	// std::mt19937 seed(device());  
	// std::uniform_real_distribution<U> distribution(-1.0, 1.0);
	// typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::NullaryExpr(size,size,[&](){return distribution(seed);});

	// utils::cast<U,T>(data,this->data);

	return;
};

template <typename T>
void Tensor<T>::eig(){
	
	std::vector<T> eigenvalues;
	Tensor<T>::type eigenvectors;

	this->eig(eigenvalues,eigenvectors);

	return;
};

template <typename T>
void Tensor<T>::eig(std::vector<T> & eigenvalues, Tensor<T>::type & eigenvectors){
	this->solver.compute(this->data);
	std::cout << this->solver.eigenvalues() << std::endl;

	std::string path;

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
void Tensor<T>::dump(){

	// Path
	std::string path = this->system.path;

	// Data
	typedef T T_data;
	io::io<T_data> io_data;
    io_data.dump(path,this->system.group,this->system.name,this->data);


    // Metadata
	typedef int T_metadata;    
	io::io<T_metadata> io_metadata;

	std::map<std::string,T_metadata> attributes;
	attributes["N"] = this->system.N;
	attributes["D"] = this->system.D;
	attributes["d"] = this->system.d;
	attributes["n"] = this->system.n;
	attributes["K"] = this->system.K;

    io_metadata.dump(path,this->system.group,this->system.name,attributes);


    // Parameters
	typedef T T_parameters;    
	io::io<T_parameters> io_parameters;

	std::map<std::string,T_parameters> parameters;
	parameters["J"] = this->system.parameters[0];
	parameters["h"] = this->system.parameters[1];
	parameters["U"] = this->system.parameters[2];

    io_parameters.dump(path,this->system.group,this->system.name,parameters);

    return;
};

template <typename T>
void Tensor<T>::load(){

	// Path
	std::string path = this->system.path;
	
	// Data
	io::io<T> io;

	std::vector<std::vector<T>> file;

	io.load(path,file);

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
// template class Tensor<float>;
template class System<double>;
// template class System<float>;

};

