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

	unsigned int n = this->system.n;
	unsigned int N = this->system.N;
	std::map<std::string,T> parameters = this->system.parameters;

	unsigned int m = N/2;

	unsigned int i,j,k;
	std::string name;
	T value;

	typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::Zero(n,n);
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;
	std::map<std::string,typename tensor::Tensor<T>::vector> state = this->state;


	#pragma omp parallel for private(i,k,name) shared(data,state)
	for (i=0;i<n;i++){

		// State
		name = "order";
		value = utils::spincount(i,N);
		states[name](i) = value;

		// ZZ Term
		for (k=0; k<N; k++){
			data(i,i) += -parameters["J"]*utils::spin(i,k)*utils::spin(i,(k+1)%N);
		};

		// if (utils::bitcount(i) == m) {
		// 	for (k=0; k<N; k++){
		// 		j = utils::flip(i,k%N);
		// 		data(i,j) += parameters["h"]*utils::bit(j,k);
		// 		// utils::bit(utils::phaseflip(i,k),k);
		// 		// utils::bit(utils::phase(i,k),k);
		// 	};
		// };
	};

	
	this->data = data;
	this->states = states;
	this->state = state;

 //    std::cout << this->data << std::endl;

	// for (i=0;i<this->system.state.size();i++){
	// 	name = this->system.state[i];
	//     std::cout << name << std::endl;
	// 	std::cout << this->states[name] << std::endl;
	// 	std::cout << this->state[name] << std::endl;
	// 	std::cout << std::endl;
	// };


	return;
};



template <typename T>
void Hamiltonian<T>::compute(){
	
	unsigned int N = this->system.N; // Number of sites
	unsigned int n = this->system.n; // Number of states
	unsigned int s = this->system.s; // Number of unique states to process
	unsigned int k;
	std::string name;
	T value;

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices;

	i = 0;
	indices.push_back(std::vector<unsigned int>());
	indices.back().push_back(i);

	for (i=1;i<n;i++){
		if (this->eigenvalues(i) > this->eigenvalues(indices.back().back())){
			indices.push_back(std::vector<unsigned int>());
			indices.back().push_back(i);			
		}
		else if (this->eigenvalues(i) == this->eigenvalues(indices.back().back())){
			indices.back().push_back(i);			
		};
	};

	// State

	for (k=0; k<s; k++){
		
		// Spin
		name = "order";
		value = 0;
		this->state[name](k) = value;

		// Energy
		name = "energy";
		value = 0;
		this->state[name](k) = value;
		

		std::cout << indices[k].size() << std::endl;

		for (i=0;i<indices[k].size();i++){

			// Spin
			name = "order";
			value = std::abs(this->eigenvectors.col(indices[k][i]).array().abs().pow(2).matrix().dot(this->states[name]))/N;
			this->state[name](k) += value/indices[k].size();

			// Energy
			name = "energy";
			value = this->eigenvalues(indices[k][i])/N;
			this->state[name](k) += value/indices[k].size();

		};

		// Gap
		name = "gap";
		value = this->eigenvalues(std::min(indices[k+1].front(),n-1)) - this->eigenvalues(indices[k].front());
		this->state[name](k) = value;

		// Entanglement
		name = "entanglement";
		value = 0;
		this->state[name](k) = 0;

	};

	return;

};

template class Hamiltonian<double>;
template class Hamiltonian<float>;


};