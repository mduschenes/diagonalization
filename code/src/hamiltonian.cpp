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

	// typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::Zero(n,n);
	typename tensor::Tensor<T>::type data(n,n);
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;
	std::map<std::string,typename tensor::Tensor<T>::vector> state = this->state;


	#pragma omp parallel for private(i,j,k,name) shared(data,state)
	for (i=0;i<n;i++){

		// State
		name = "order";
		value = utils::spincount(i,N);
		states[name](i) = value;

		// ZZ Term
		for (k=0; k<N; k++){
			j = i;
			value = -parameters["J"]*utils::spin(i,k)*utils::spin(j,(k+1)%N);
			data(i,j) += value;		
			// data.coeffRef(i,j) += value;	
		};

		// if (utils::bitcount(i) == m) {
		// 	for (k=0; k<N; k++){
		// 		j = utils::flip(i,k%N);
		// 		value = parameters["h"]*utils::bit(j,k);
		// 		data(i,j) += value;
		// 		data.insert(i,j) += value;
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
	unsigned int q = this->system.q; // Number of states to process
	unsigned int k;
	std::string name;
	T value;

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices;

	i = 0;
	indices.push_back(std::vector<unsigned int>());
	indices.back().push_back(i);

	for (i=1;i<q;i++){
		if (std::abs(this->eigenvalues(i) - this->eigenvalues(indices.back().back())) >= this->system.eps){
			indices.push_back(std::vector<unsigned int>());
			indices.back().push_back(i);			
		}
		else if (std::abs(this->eigenvalues(i) - this->eigenvalues(indices.back().back())) < this->system.eps){
			indices.back().push_back(i);			
		};
	};

	std::cout << this->eigenvalues << std::endl;
	std::cout << "-------------" << std::endl;

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
		
		for (i=0;i<indices[k].size();i++){


			std::cout << this->eigenvectors.col(indices[k][i]).cwiseAbs2() << std::endl;
			std::cout << "--" << std::endl;
			std::cout << this->states["order"] << std::endl;
			std::cout << std::endl;

			// Spin
			name = "order";
			value = std::abs(this->eigenvectors.col(indices[k][i]).cwiseAbs2().dot(this->states[name]))/N;
			this->state[name](k) += value/indices[k].size();

			// Energy
			name = "energy";
			value = this->eigenvalues(indices[k][i])/N;
			this->state[name](k) += value/indices[k].size();

		};

		// Gap
		name = "gap";
		value = this->eigenvalues(indices[std::min(k+1,s-1)].front()) - this->eigenvalues(indices[k].front());
		this->state[name](k) = value;

		// Entanglement
		name = "entanglement";
		value = 0;
		this->state[name](k) = 0;

	};

	// Check
	for (i=0;i<this->system.state.size();i++){
		utils::check(this->state[this->system.state[i]],this->system.eps);
	};

	return;

};

template class Hamiltonian<double>;
template class Hamiltonian<float>;


};