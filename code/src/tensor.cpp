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
void Tensor<T>::eig(){

	// Solve

	// int q = this->system.q; // Number of eigenvalues
	// int r =  std::min(3*this->system.q,this->system.size); // Solver tolerance

	// T shift = 1*((this->system.parameters["J"]/2.0)*this->system.z*this->system.N);
	// T factor = -1;
	// this->data.array() *= factor;
	// this->data.diagonal().array() += shift;
	// utils::check(this->data,this->system.eps);

	// // this->data.coeffs() *= factor;
	// // this->data.diagonal().array() += shift;
	// // utils::check(this->data,this->system.eps);

	// std::cout << shift << std::endl;
	// std::cout << this->data << std::endl;

	// typename tensor::Tensor<T>::op op(this->data);
	// typename tensor::Tensor<T>::solver solver(op,q,r);

	// solver.init();
	// solver.compute(this->sort);
 
	// this->eigenvalues = solver.eigenvalues();
	// this->eigenvectors = solver.eigenvectors();

	// this->eigenvalues.array() -= shift;
	// this->eigenvalues.array() /= factor;
	// utils::check(this->eigenvalues,this->system.eps);
	// utils::check(this->eigenvectors,this->system.eps);

	// this->data.diagonal().array() -= shift;
	// this->data.array() /= factor;
	// utils::check(this->data,this->system.eps);

	// // this->data.diagonal().array() -= shift;
	// // this->data.coeffs() /= factor;
	// // utils::check(this->data,this->system.eps);	

	// Solve

	typename tensor::Tensor<T>::solver solver;
	solver.compute(this->data);
	
	this->eigenvalues = solver.eigenvalues();
	this->eigenvectors = solver.eigenvectors();

	utils::check(this->eigenvalues,this->system.eps);
	utils::check(this->eigenvectors,this->system.eps);


	// Dump
	// io::io<T> io;
	// std::string path = this->system.path;
	// std::string group = this->system.group;
	// std::string name = this->system.name;
	
	// name = "eigenvalues";
	// io.dump(path,group,name,this->eigenvalues);

	// name = "eigenvectors";
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
	std::cout << "|data| = " << utils::norm<T>(this->data,this->data) << std::endl;
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


	// Attributes
	typedef int T_attributes;    
	io::io<T_attributes> io_attributes;
	std::map<std::string,T_attributes> attributes = {
		{"N",this->system.N},{"D",this->system.D},{"d",this->system.d},
		{"n",this->system.n},{"k",this->system.k},{"s",this->system.s},
	};
	io_attributes.dump(path,group,name,attributes);


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
	
	typename tensor::Tensor<T>::type data(size,size);

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			try {
				data(i,j) = file[i][j];
			}
			catch (...){
				data.coeffRef(i,j) = file[i][j];
			};
		};
	};

	this->data = data;

};

template class Tensor<double>;
template class Tensor<float>;

template class System<double>;
template class System<float>;

};

