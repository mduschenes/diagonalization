#include "tensor.hpp"

namespace tensor {

// Constructor and Destructor
template <typename T>
Tensor<T>::Tensor(tensor::System<T> & system) : size(system.size), dim(system.dim), nnz(system.nnz), sparse(system.sparse){
	this->system = system;
};

template <typename T>
Tensor<T>::~Tensor(){
};




template <typename T>
void Tensor<T>::eig(){

	unsigned int n = std::min(this->size-1,this->system.q); // Number of eigenvalues
	std::string sigma = this->system.sigma; // Eigenvalue to centre around
	T eps = this->system.eps; // Eigenvalues tolerance

	T scale = this->system.scale; // Scale data
	T shift = this->system.shift; // Shift diagonal of data

	// this->data.array() *= scale;
	this->data.coeffs() *= scale;
	this->data.diagonal().array() += shift;

	linalg::eigh(this->data,this->eigenvalues,this->eigenvectors,n,sigma,eps);

	this->eigenvalues.array() -= shift;
	this->eigenvalues.array() /= scale;

	this->data.diagonal().array() -= shift;
	this->data.coeffs() /= scale;
	// this->data.array() /= scale;

	return;
};



template <typename T>
void Tensor<T>::print(){
	std::cout << "group = " << this->system.group << std::endl;
	std::cout << "model = " << this->system.model << std::endl;
	std::cout << "size = " << this->size << "\t";
	std::cout << "dim = " << this->dim << "\t";
	std::cout << "sparse = " << this->sparse << std::endl;
	std::cout << "N = " << this->system.N << "\t";
	std::cout << "D = " << this->system.D << "\t";
	std::cout << "d = " << this->system.d << "\t";
	std::cout << "n = " << this->system.n << std::endl;
	std::cout << "J = " << this->system.parameters["J"] << "\t";
	std::cout << "h = " << this->system.parameters["h"] << "\t";
	std::cout << "U = " << this->system.parameters["U"] << std::endl;
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
	group = this->system.group;
	name = "data";
	// io_data.dump(path,group,name,this->data);

	// Attributes
	typedef unsigned int T_attributes;    
	io::io<T_attributes> io_attributes;
	std::map<std::string,T_attributes> attributes = {
		{"N",this->system.N},{"D",this->system.D},{"d",this->system.d},
		{"n",this->system.n},{"k",this->system.k},{"s",this->system.s},
	};
	group = this->system.group;
	name = this->system.name;	

	io_attributes.dump(path,group,name,attributes);

	// Parameters
	typedef T T_parameters;    
	io::io<T_parameters> io_parameters;
	group = this->system.group;
	name = this->system.name;
	io_parameters.dump(path,group,name,this->system.parameters);

	// State
	typedef T T_state;
	io::io<T_state> io_state;
	group = this->system.group;
	name = this->system.name;
	io_state.dump(path,group,name,this->state);

	// Eigenvalues
	typedef T T_eig;
	io::io<T_eig> io_eig;
	group = this->system.group;
	name = "eigenvalues";
	io_eig.dump(path,group,name,this->eigenvalues);
	group = this->system.group;
	name = "eigenvectors";
	// io_eig.dump(path,group,name,this->eigenvectors);

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

	unsigned int size = this->size;
	
	typename tensor::Tensor<T>::type data(size,size);

	for (unsigned int i = 0; i < size; i++){
		for (unsigned int j = 0; j < size; j++){
			data.coeffRef(i,j) = file[i][j];
		};
	};

	this->data = data;

};

template class Tensor<double>;
template class Tensor<float>;

template class System<double>;
template class System<float>;

};

