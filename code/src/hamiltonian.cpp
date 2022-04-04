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
	unsigned int nnz = this->system.nnz;
	std::map<std::string,T> parameters = this->system.parameters;

	std::vector<int> subspaces;
	// subspaces.push_back(0);
	subspaces.resize(2*N+1);
	std::iota(subspaces.begin(),subspaces.end(),-N);

	int subspace;
	unsigned int size;
	std::vector<unsigned int> indices;

	unsigned int i,j,k,s,t;
	std::string name;
	T value;

	// typename tensor::Tensor<T>::type data = tensor::Tensor<T>::type::Zero(n,n);
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;

	for (i=0;i<n;i++){
		subspace = utils::spincount(i,N);
		if (utils::isin(subspaces,subspace)) {
			indices.push_back(i);
		};
	};
	size = indices.size();

	typename tensor::Tensor<T>::type data(size,size);
	data.reserve(nnz);
	
	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		states[name] = tensor::Tensor<T>::vector::Zero(size);
	};

	// typename typedef tensor::Tensor<T>::index index;
	// typename std::vector<index> indexes;
	// indexes.push_back(index(i,j,value));
	// data.setFromTriplets(indexes.begin(),indexes.end());



	// this->parallel();
	// #pragma omp parallel for private(i,j,k,s,t,name) shared(data,states,parameters)
	for (s=0;s<size;s++){

		// Index
		i = indices[s];

		// State
		name = "order";
		value = utils::spincount(i,N);
		states[name](s) = value;

		// ZZ Term
		for (k=0; k<N; k++){
			j = i;
			t = s;
			value = -parameters["J"]*utils::spin(i,k)*utils::spin(j,(k+1)%N);
			data.coeffRef(s,t) += value;	
		};

		// X Term
		// 	for (k=0; k<N; k++){
		// 		j = utils::flip(i,k%N);
		// if (utils::isin(subspaces,j)){
		// 	t = utils::find(subspaces,j);
		// };
		// 		value = parameters["h"]*utils::bit(j,k);
		// 		data.coeffRef(s,t) += value;
		// 		// utils::bit(utils::phaseflip(i,k),k);
		// 		// utils::bit(utils::phase(i,k),k);
		// 	};
	};


	this->size = size;
	this->data = data;

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		this->states[name] = states[name];
	};


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

	// Sort eigenvalues
	std::vector<int> argsort; // Sorting indices of eigenvalues
	std::string sorting = this->system.sorting; // Sorting algorithm of eigenvalues
	int axis = 1; // Axis of eigenvectors for sorting
	T eps = this->system.eps; // Tolerance of closeness of sorted eigenvalues

	utils::check(this->eigenvalues,eps);
	utils::check(this->eigenvectors,eps);
	utils::argsort(this->eigenvalues,argsort,sorting);
	utils::permute(this->eigenvalues,argsort);
	utils::permute(this->eigenvectors,argsort,axis);
	utils::check(this->eigenvalues,eps);
	utils::check(this->eigenvectors,eps);

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices; // vector of vectors of grouped indices of equal eigenvectors
	std::vector<unsigned int> size; // size of each group of equal eigenvectors

	q = std::min(std::min((unsigned int)(this->eigenvalues.size()),(unsigned int)(this->eigenvectors.cols())),q);
	for (i=0;i<q;i++){
		if ((i==0) or (not utils::close(this->eigenvalues(i),this->eigenvalues(indices.back().back()), eps))){
			indices.push_back(std::vector<unsigned int>());
			indices.back().push_back(i);			
			size.push_back(0);
		}
		else {
			indices.back().push_back(i);			
		};
		size.back()++;			
	};


	// State
	s = std::min((unsigned int)(indices.size()),s);
	std::map<std::string,typename tensor::Tensor<T>::vector> state = this->state;
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		state[name] = tensor::Tensor<T>::vector::Zero(s);
	};

	for (k=0; k<s; k++){

		// Order
		name = "order";
		value = 0;
		state[name](k) = value;
	
		// Energy
		name = "energy";
		value = 0;
		state[name](k) = value;
		
		for (i=0;i<size[k];i++){

			// Order
			name = "order";
			value = std::abs(this->eigenvectors.col(indices[k][i]).cwiseAbs2().dot(this->states[name].cwiseAbs()))/N;
			state[name](k) += value/size[k];

			// Energy
			name = "energy";
			value = this->eigenvalues(indices[k][i])/N;
			state[name](k) += value/size[k];

		};

		// Gap
		name = "gap";
		value = this->eigenvalues(indices[std::min(k+1,s-1)].front()) - this->eigenvalues(indices[k].front());
		state[name](k) = value;

		// Entanglement
		name = "entanglement";
		value = 0;
		state[name](k) = 0;

	};


	// Check
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		utils::check(state[name],eps);
		this->state[name] = state[name];
	};

	return;

};

template class Hamiltonian<double>;
template class Hamiltonian<float>;


};