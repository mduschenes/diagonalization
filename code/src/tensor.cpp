#include "tensor.hpp"

namespace tensor {

// Constructor and Destructor
template <typename T>
Tensor<T>::Tensor(tensor::System<T> & system) : size(system.size), dim(system.dim), nnz(system.nnz), sparse(system.sparse){
	this->setup(system);
};

template <typename T>
Tensor<T>::~Tensor(){
};


template <typename T>
void Tensor<T>::setup(tensor::System<T> & system){
	
	unsigned int i;
	unsigned int size = this->size;
	unsigned int nnz = this->nnz;
	unsigned int s = system.s;
	std::string name;

	for (i=0;i<system.state.size();i++){
		name = system.state[i];
		this->states[name] = tensor::Tensor<T>::vector::Zero(size);
		this->state[name] = tensor::Tensor<T>::vector::Zero(s);		
	};

	this->system = system;

	if (this->system.space == "spin"){
		this->system.n = pow(this->system.D,this->system.N);
	}
	else {
		this->system.n = pow(this->system.D,this->system.N);		
	};

	if (this->system.lattice == "square"){
		this->system.z = 2*this->system.d;
	}
	else {
		this->system.z = 2*this->system.d;
	};

	// this->data = tensor::Tensor<T>::type(size,size);
	// this->data.reserve(nnz);
};




template <typename T>
void Tensor<T>::eig(){

	// Solve

	// unsigned int q = std::min(this->size-1,this->system.q); // Number of eigenvalues
	// unsigned int r =  std::min(3*this->system.q,this->size); // Solver tolerance

	// T shift = 1*((this->system.parameters["J"]/2.0)*this->system.z*this->system.N);
	// T scale = -1;
	// this->data.array() *= scale;
	// this->data.diagonal().array() += shift;
	// utils::check(this->data,this->system.eps);

	// // this->data.coeffs() *= scale;
	// // this->data.diagonal().array() += shift;
	// // utils::check(this->data,this->system.eps);

	// typename tensor::Tensor<T>::op op(this->data);
	// typename tensor::Tensor<T>::solver solver(op,q,r);

	// solver.init();
	// solver.compute(this->sort);
 
	// this->eigenvalues = solver.eigenvalues();
	// this->eigenvectors = solver.eigenvectors();

	// this->eigenvalues.array() -= shift;
	// this->eigenvalues.array() /= scale;
	// utils::check(this->eigenvalues,this->system.eps);
	// utils::check(this->eigenvectors,this->system.eps);

	// this->data.diagonal().array() -= shift;
	// this->data.array() /= scale;
	// utils::check(this->data,this->system.eps);

	// // this->data.diagonal().array() -= shift;
	// // this->data.coeffs() /= scale;
	// // utils::check(this->data,this->system.eps);	


	// Solve

	unsigned int q = std::min(this->size-1,this->system.q); // Number of eigenvalues
	std::string sigma = this->system.sigma; // Eigenvalue to centre around
	// std::string sigma = utils::string((this->system.parameters["J"]/2.0)*this->system.z); // Eigenvalue to centre around
	// std::string sigma = utils::string(this->system.sigma); // Eigenvalue to centre around
	int options = -1; // Eigenvalues options
	T eps = this->system.eps; // Eigenvalues tolerance

	// T scale = -1.0/this->system.N;
	T scale = -1.0; // /(T)(this->system.N);
	T shift = this->system.tol*((this->system.parameters["J"]/2.0)*this->system.z*this->system.N);

	// this->data.array() *= scale;
	this->data.coeffs() *= scale;
	this->data.diagonal().array() += shift;

	typename tensor::Tensor<T>::solver solver;
	solver.compute(this->data,q,sigma,options,eps); // https://docs.scipy.org/doc/scipy/tutorial/arpack.html
	// solver.compute(this->data);

	this->eigenvalues = solver.eigenvalues();
	this->eigenvectors = solver.eigenvectors();

	this->eigenvalues.array() -= shift;
	this->eigenvalues.array() /= scale;

	this->data.diagonal().array() -= shift;
	this->data.coeffs() /= scale;
	// this->data.array() /= scale;


	// Solve

	// typename tensor::Tensor<T>::solver solver;
	// solver.compute(this->data);
	
	// this->eigenvalues = solver.eigenvalues();
	// this->eigenvectors = solver.eigenvectors();

	return;
};



template <typename T>
void Tensor<T>::print(){
	std::cout << "group = " << this->system.group << std::endl;
	std::cout << "N = " << this->system.N << std::endl;
	std::cout << "D = " << this->system.D << std::endl;
	std::cout << "d = " << this->system.d << std::endl;
	std::cout << "n = " << this->system.n << std::endl;
	std::cout << "k = " << this->system.k << std::endl;
	std::cout << "J = " << this->system.parameters["J"] << std::endl;
	std::cout << "h = " << this->system.parameters["h"] << std::endl;
	// std::cout << "|data| = " << utils::norm<T>(this->data,this->data) << std::endl;
	// std::cout << "data = \n" << this->data << std::endl;
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

