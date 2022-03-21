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
	
	unsigned int i;
	unsigned int size = this->size;
	unsigned int s = system.s;
	std::string name;

	for (i=0;i<system.state.size();i++){
		name = system.state[i];
		this->states[name] = tensor::Tensor<T>::vector::Zero(size);
		this->state[name] = tensor::Tensor<T>::vector::Zero(s);		
	};

	this->system = system;

};



template <typename T>
void Tensor<T>::rand(){

	int size = this->size;
	int dim = this->dim;

	int N = this->system.N;
	int D = this->system.D;
	int n = this->system.n;
	
	typedef T U;

	std::random_device device;
	std::mt19937 seed(device());  
	std::uniform_real_distribution<U> distribution(-1.0, 1.0);
	typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::NullaryExpr(size,size,[&](){return distribution(seed);});

	utils::cast<U,T>(data,this->data);

	return;
};

template <typename T>
void Tensor<T>::eig(){

	// Solve

	// this->op(this->data);    
	// this->solver(this->op, this->system.s, 2*this->system.s);
	// this->solver.init();
	// this->solver.compute(Spectra::SortRule::SmallestMagn);

	this->solver.compute(this->data);

	this->eigenvalues = this->solver.eigenvalues();
	this->eigenvectors = this->solver.eigenvectors();

	utils::check(this->eigenvalues);
	utils::check(this->eigenvectors);

	
	// Dump
	io::io<T> io;
	std::string path = this->system.path;
	std::string group = this->system.group;
	std::string name = this->system.name;
	
	name = "eigenvalues";
    // io.dump(path,group,name,this->eigenvalues);

	name = "eigenvectors";
    // io.dump(path,group,name,this->eigenvectors);

	return;
};



template <typename T>
void Tensor<T>::print(){
	std::cout << "N = " << this->system.N << std::endl;
	std::cout << "D = " << this->system.D << std::endl;
	std::cout << "d = " << this->system.d << std::endl;
	std::cout << "n = " << this->system.n << std::endl;
	std::cout << "k = " << this->system.k << std::endl;
	std::cout << "|O| = " << utils::norm<T>(this->data,this->data) << std::endl;
	std::cout << "data = \n" << this->data << std::endl;
	std::cout << std::endl;	
};



template <typename T>
void Tensor<T>::dump(){

	// Path
	std::string path = this->system.path;
	std::string group = this->system.group;
	std::string name = this->system.name;

	// Data
	typedef T T_data;
	io::io<T_data> io_data;

    // io_data.dump(path,group,name,this->data);


    // Metadata
	typedef int T_metadata;    
	io::io<T_metadata> io_metadata;

	std::vector<T_metadata> variables = {this->system.N,this->system.D,this->system.d,this->system.n,this->system.k,this->system.s};
	std::map<std::string,T_metadata> attributes;
	for (int i=0;i<this->system.k;i++){
		attributes[this->system.names[i]] = variables[i];
	};

    io_metadata.dump(path,group,name,attributes);


    // Parameters
	typedef T T_parameters;    
	io::io<T_parameters> io_parameters;

    io_parameters.dump(path,group,name,this->system.parameters);


    // State
	typedef T T_state;
	io::io<T_state> io_state;

    io_state.dump(path,group,name,this->state);

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
template class Tensor<float>;
template class System<double>;
template class System<float>;

};

