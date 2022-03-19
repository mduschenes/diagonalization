#include "hamiltonian.hpp"

namespace hamiltonian {

	// Constructor and Destructor
template <typename T>
Hamiltonian<T>::Hamiltonian(tensor::System<T> & system) : tensor::Tensor<T>(system){
};

template <typename T>
Hamiltonian<T>::~Hamiltonian(){
};



template <typename T>
void Hamiltonian<T>::set(){

	unsigned int i,j,k;
	unsigned int n = this->system.n;
	unsigned int N = this->system.N;
	std::vector<T> parameters = this->system.parameters;

	unsigned int m = N/2;

	typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::Zero(n,n);

	// #pragma omp parallel for private(i,j,k) shared(data)
	for (i=0;i<n;i++){
		if (utils::bitcount(i) == m) {
			for (k=0; k<N; k++){
				j = utils::flip(i,k%N);
				data(i,j) += parameters[1]*utils::bit(j,k);
				// utils::bit(utils::phaseflip(i,k),k);
				// utils::bit(utils::phase(i,k),k);
			};
		};
	};

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			std::cout << data(i,j) << " ";
		};
		std::cout << std::endl;
	};

	return;
};


// template <typename T>
// void Tensor<T>::set(){

// 	int size = this->size;
// 	int dim = this->dim;

// 	int N = this->system.N;
// 	int D = this->system.D;
// 	int n = this->system.n;

// 	// Tensor<T>::type data(size,size);
// 	// T value = 0;
	
// 	// for (unsigned int i=0; i<(pow(size,dim)); i++){
// 	// 	data(i) =  value;
// 	// };
// 	// this->data = data;

// 	this->rand();
// };


template class Hamiltonian<double>;
// template class Hamiltonian<float>;


};