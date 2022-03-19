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
	unsigned int s = this->system.s;
	unsigned int N = this->system.N;
	std::vector<T> parameters = this->system.parameters;

	unsigned int m = N/2;

	typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::Zero(n,n);
	typename tensor::Tensor<T>::matrix states = tensor::Tensor<T>::matrix::Zero(n,s);
	typename tensor::Tensor<T>::vector state = tensor::Tensor<T>::vector::Zero(s);

	#pragma omp parallel for private(i,k) shared(data,states)
	for (i=0;i<n;i++){

		// State
		for (k=0; k<s; k++){
			switch (k){
				// case :0 case: 1 case: 2:
				case 0 ... 2:
					states(i,k) = utils::spincount(i,N);
					break;
				default:
					break;
			};
		};

		// ZZ Term
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
	this->states = states;
	this->state = state;

    // std::cout << this->data << std::endl;
    // std::cout << this->states.col(0) << std::endl;


	return;
};



template <typename T>
void Hamiltonian<T>::compute(){
	
	unsigned int k;
	unsigned int n = this->system.n;
	unsigned int s = this->system.s;

	// Unique sorted eigenvalue indices
	unsigned int i,j;
	std::vector<std::vector<int>> indices;

	i = 0;
	indices.push_back(std::vector<int>());
	indices.back().push_back(i);

	for (i=1;i<n;i++){
		if (this->eigenvalues(i) > this->eigenvalues(indices.back().back())){
			indices.push_back(std::vector<int>());
			indices.back().push_back(i);			
		}
		else if (this->eigenvalues(i) == this->eigenvalues(indices.back().back())){
			indices.back().push_back(i);			
		};
	};

	// State
	for (k=0; k<s; k++){
		switch (k){

			// Ground ... Second State Spin
			case 0 ... 2:
				this->state(k) = this->eigenvectors.col(indices[k].front()).array().abs().pow(2).matrix().dot(this->states.col(k));
				break;

			// Ground ... Second State Energy
			case 3 ... 5:
				this->state(k) = this->eigenvalues(indices[k-3].front());
				break;

			// Ground State Gap
			case 6:
				this->state(k) = this->eigenvalues(indices[1].front()) - this->eigenvalues(indices[0].front());
				break;

			default:
				break;
		};
	};

	return;

};

template class Hamiltonian<double>;
template class Hamiltonian<float>;


};