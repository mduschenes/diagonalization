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
void Hamiltonian<T>::set(std::vector<T> & theta){

	unsigned int i,j,k;
	unsigned int n = this->system.n;
	unsigned int N = this->system.N;

	typename decltype(*this)::Type data = decltype(*this)::Type::Zero(n,n);


	#pragma omp parallel for private(i,j,k) shared(data)
	for (i=0;i<size;i++){
		if (utils::bitcount(i) == m) {
			for (k=0; k<n; k++){
				j = utils::flip(i,k);
				data(i,j) += theta[0]*utils::bit(j,k);
				// utils::bit(utils::phaseflip(i,k),k);
				// utils::bit(utils::phase(i,k),k);
			};
		};
	};

	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			std::cout << data(i,j) << " ";
		};
		std::cout << std::endl;
	};
};


// template <typename T>
// void Tensor<T>::set(std::vector<T> & theta){

// 	int size = this->size;
// 	int dim = this->dim;

// 	int N = this->system.N;
// 	int D = this->system.D;
// 	int n = this->system.n;

// 	// Tensor<T>::Type data(size,size);
// 	// T value = 0;
	
// 	// for (unsigned int i=0; i<(pow(size,dim)); i++){
// 	// 	data(i) =  value;
// 	// };
// 	// this->data = data;

// 	this->rand(theta);
// };


template class Hamiltonian<double>;
template class Hamiltonian<float>;


};