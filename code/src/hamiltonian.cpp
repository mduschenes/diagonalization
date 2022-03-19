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

	#pragma omp parallel for private(i,k) shared(data)
	for (i=0;i<n;i++){

		for (k=0; k<N; k++){
			data(i,i) += -parameters[0]*utils::spin(i,k)*utils::spin(i,(k+1)%N);
		};

		// if (utils::bitcount(i) == m) {
		// 	for (k=0; k<N; k++){
		// 		j = utils::flip(i,k%N);
		// 		data(i,j) += parameters[1]*utils::bit(j,k);
		// 		// utils::bit(utils::phaseflip(i,k),k);
		// 		// utils::bit(utils::phase(i,k),k);
		// 	};
		// };
	};

	
	this->data = data;

    std::cout << this->data << std::endl;


	return;
};


template class Hamiltonian<double>;
template class Hamiltonian<float>;


};