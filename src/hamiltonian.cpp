#include "hamiltonian.hpp"

namespace hamiltonian {

// Constructor and Destructor
template <typename T>
Hamiltonian<T>::Hamiltonian(tensor::System<T> & system) : tensor::Tensor<T>(system){
	this->setup(system);
};

template <typename T>
Hamiltonian<T>::~Hamiltonian(){
};


template <typename T>
void Hamiltonian<T>::setup(tensor::System<T> & system){
	
	unsigned int i;
	typename std::map<std::string,std::vector<T>>::iterator iterator;	
	unsigned int size = this->size;
	unsigned int nnz = this->nnz;
	unsigned int N = system.N;
	unsigned int D = system.D;
	unsigned int s = system.s;
	std::string name;

	for (i=0;i<system.state.size();i++){
		name = system.state[i];
		this->states[name];
		this->state[name];
	};

	for (iterator=system.symmetries.begin();iterator!=system.symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			if (system.symmetries[name].size() == 0){
				system.symmetries[name].resize(D*N+1);
				std::iota(system.symmetries[name].begin(),system.symmetries[name].end(),-1.0*N);
			};
		};
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

};



template <typename T>
void Hamiltonian<T>::set(){

	unsigned int i,j,k,s,t;
	std::string name;
	T value;

	unsigned int size;
	typename std::map<std::string,std::vector<T>>::iterator iterator;
	std::vector<unsigned int> subspaces;
	std::map<unsigned int,int> included;	
	T symmetry;
	std::map<std::string,std::vector<T>> symmetries = this->system.symmetries;

	unsigned int n = this->system.n;
	unsigned int N = this->system.N;
	unsigned int nnz = this->system.nnz;
	std::map<std::string,T> parameters = this->system.parameters;
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;

	for (iterator=symmetries.begin();iterator!=symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			for (i=0;i<n;i++){
				symmetry = physics::spincount(i,N);
				if (utils::isin(symmetries[name],symmetry)){
					subspaces.push_back(i);
					included[i] = subspaces.size()-1;
				}
				else{
					included[i] = -1;
				};
			};		
		};		
	};

	size = subspaces.size();
	nnz = std::min(nnz,size*size);

	typename tensor::Tensor<T>::type data(size,size);
	data.reserve(nnz);

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		if ((name == "order")){
			states[name] = tensor::Tensor<T>::vector::Zero(size);
		}
		else if ((name == "energy") or (name == "gap")){
			states[name];
		}
		else if ((name == "entanglement") or (name == "partition")){
			states[name];
		}
		else {
			states[name];
		};		
	};

	typedef typename tensor::Tensor<T>::index index;
	std::vector<index> indexes;


	this->parallel();
	#pragma omp parallel for private(i,j,k,s,t,name,value) shared(states,indexes,subspaces,included)
	for (s=0;s<size;s++){

		// Index
		i = subspaces[s];

		// States
		// for (i=0;i<this->system.state.size();i++){
		// 	name = this->system.state[i];
		// 	if ((name == "order")){
		// 		value = physics::spincount(i,N);
		// 		states[name](s) = value;
		// 	}
		// };

		// Order
		name = "order";
		value = physics::spincount(i,N);
		states[name](s) = value;


		// ZZ Term
		for (k=0; k<N; k++){
			j = i;
			t = s;
			value = -parameters["J"]*physics::spin(i,(k)%N)*physics::spin(j,(k+1)%N);

			#pragma omp critical
			indexes.push_back(index(s,t,value));
		};

		// X Term
		for (k=0; k<N; k++){
			j = physics::spinflip(i,(k)%N);
			if (included[j] != -1){
				t = included[j];
				value = -parameters["h"];

				#pragma omp critical
				indexes.push_back(index(s,t,value));
			};
		};
	};

	data.setFromTriplets(indexes.begin(),indexes.end());

	this->size = size;
	this->nnz = nnz;
	this->data = data;

	this->subspaces = subspaces;
	this->included = included;

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		this->states[name] = states[name];
	};

	return;
};



template <typename T>
void Hamiltonian<T>::compute(){
	
	unsigned int N = this->system.N; // Number of sites
	unsigned int D = this->system.D; // Dimension of sites
	unsigned int n = this->system.n; // Number of states
	unsigned int s = this->system.s; // Number of unique states to process
	unsigned int q = this->system.q; // Number of states to process
	unsigned int j,k,l;
	std::string name;
	T value;
	typename tensor::Tensor<T>::vector_complex eigenvector;

	// Sort eigenvalues
	std::vector<int> argsort; // Sorting indices of eigenvalues
	std::string sorting = this->system.sorting; // Sorting algorithm of eigenvalues
	int axis = 1; // Axis of eigenvectors for sorting
	T eps = this->system.eps; // Floating point tolerance
	T tol = this->system.tol; // Tolerance of closeness of sorted eigenvalues
	T close = this->system.close; // Tolerance of closeness of sorted eigenvalues

	utils::check(this->eigenvalues,eps);
	utils::check(this->eigenvectors,eps);
	linalg::argsort(this->eigenvalues,argsort,sorting);
	linalg::permute(this->eigenvalues,argsort);
	linalg::permute(this->eigenvectors,argsort,axis);

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices; // vector of vectors of grouped indices of equal eigenvectors
	std::vector<unsigned int> size; // size of each group of equal eigenvectors

	q = std::min(std::min((unsigned int)(this->eigenvalues.size()),(unsigned int)(this->eigenvectors.cols())),q);
	for (i=0;i<q;i++){

		if ((size.size() == 1) and (this->system.parameters["h"] == 0)){
			tol = close;
		}
		else {
			tol = this->system.tol;
		};
		if ((i==0) or (not utils::close(this->eigenvalues(i),this->eigenvalues(indices.back().back()),tol))){
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
	std::map<std::string,typename tensor::Tensor<T>::matrix> state = this->state;
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		if ((name == "order") or (name == "energy") or (name == "gap")){
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		}
		else if ((name == "entanglement") or (name == "partition")){
			state[name] = tensor::Tensor<T>::matrix::Zero(s,N-1);
		}
		else {
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		};
	};


	for (k=0; k<s; k++){

		// Equal Superposition of degenerate eigenvectors in full space from subspace
		eigenvector = tensor::Tensor<T>::vector_complex::Zero(n);
		for (i=0;i<size[k];i++){
			if (this->subspaces.size() == n and false){
				eigenvector = this->eigenvectors.col(indices[k][i]);
			}
			else {
				for (j=0;j<this->subspaces.size();j++){
					eigenvector(this->subspaces[j]) += this->eigenvectors(j,indices[k][i]);
				};
			};
		};	
		eigenvector /= std::sqrt(size[k]);

		// Order
		name = "order";
		value = physics::expectation(eigenvector,this->states[name],eps)/N;
		state[name](k) = value;

		// Energy
		name = "energy";
		value = this->eigenvalues(indices[k]).sum()/size[k]/N;
		state[name](k) = value;

		// Gap
		name = "gap";
		value = (this->eigenvalues(indices[std::min(k+1,s-1)].front()) - this->eigenvalues(indices[k].front()))/N;
		state[name](k) = value;

		// Entanglement
		name = "entanglement";

		// #pragma omp parallel for private(j,l,value,eigenvector,eps) shared(k,name,state)
		for (j=0;j<state[name].cols();j++){
			l = pow(D,j+1);
			value = physics::entropy(eigenvector,l,eps);

			// #pragma omp critical
			state[name](k,j) = value;
		};
		
		// Partition
		name = "partition";

		for (j=0;j<state[name].cols();j++){
			state[name](k,j) = ((T)j+1)/((T)N);
		};

	};


	// Check
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		utils::check(state[name],eps);
		this->state[name] = state[name];
	};

	return;

};

// Constructor and Destructor
template <typename T>
Ising<T>::Ising(tensor::System<T> & system) : tensor::Tensor<T>(system){
	this->setup(system);	
};

template <typename T>
Ising<T>::~Ising(){
};


template <typename T>
void Ising<T>::setup(tensor::System<T> & system){
	
	unsigned int i;
	typename std::map<std::string,std::vector<T>>::iterator iterator;	
	unsigned int size = this->size;
	unsigned int nnz = this->nnz;
	unsigned int N = system.N;
	unsigned int D = system.D;
	unsigned int s = system.s;
	std::string name;

	for (i=0;i<system.state.size();i++){
		name = system.state[i];
		this->states[name];
		this->state[name];
	};

	for (iterator=system.symmetries.begin();iterator!=system.symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			if (system.symmetries[name].size() == 0){
				system.symmetries[name].resize(D*N+1);
				std::iota(system.symmetries[name].begin(),system.symmetries[name].end(),-1.0*N);
			};
		};
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

};



template <typename T>
void Ising<T>::set(){

	unsigned int i,j,k,s,t;
	std::string name;
	T value;

	unsigned int size;
	typename std::map<std::string,std::vector<T>>::iterator iterator;
	std::vector<unsigned int> subspaces;
	std::map<unsigned int,int> included;	
	T symmetry;
	std::map<std::string,std::vector<T>> symmetries = this->system.symmetries;

	unsigned int n = this->system.n;
	unsigned int N = this->system.N;
	unsigned int nnz = this->system.nnz;
	std::map<std::string,T> parameters = this->system.parameters;
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;

	for (iterator=symmetries.begin();iterator!=symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			for (i=0;i<n;i++){
				symmetry = physics::spincount(i,N);
				if (utils::isin(symmetries[name],symmetry)){
					subspaces.push_back(i);
					included[i] = subspaces.size()-1;
				}
				else{
					included[i] = -1;
				};
			};		
		};		
	};

	size = subspaces.size();
	nnz = std::min(nnz,size*size);

	typename tensor::Tensor<T>::type data(size,size);
	data.reserve(nnz);

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		if ((name == "order") or (name == "energy") or (name == "gap")){
			states[name] = tensor::Tensor<T>::vector::Zero(size);
		}
		else if ((name == "energy") or (name == "gap")){
			states[name];
		}
		else if ((name == "entanglement") or (name == "partition")){
			states[name];
		}
		else {
			states[name];
		};		
	};

	typedef typename tensor::Tensor<T>::index index;
	std::vector<index> indexes;


	this->parallel();
	#pragma omp parallel for private(i,j,k,s,t,name,value) shared(states,indexes,subspaces,included)
	for (s=0;s<size;s++){

		// Index
		i = subspaces[s];

		// States
		// for (i=0;i<this->system.state.size();i++){
		// 	name = this->system.state[i];
		// 	if ((name == "order")){
		// 		value = physics::spincount(i,N);
		// 		states[name](s) = value;
		// 	}
		// };

		// Order
		name = "order";
		value = physics::spincount(i,N);
		states[name](s) = value;


		// ZZ Term
		for (k=0; k<N; k++){
			j = i;
			t = s;
			value = -parameters["J"]*physics::spin(i,(k)%N)*physics::spin(j,(k+1)%N);

			#pragma omp critical
			indexes.push_back(index(s,t,value));
		};

		// X Term
		for (k=0; k<N; k++){
			j = physics::spinflip(i,(k)%N);
			if (included[j] != -1){
				t = included[j];
				value = -parameters["h"];

				#pragma omp critical
				indexes.push_back(index(s,t,value));
			};
		};
	};

	data.setFromTriplets(indexes.begin(),indexes.end());

	this->size = size;
	this->nnz = nnz;
	this->data = data;

	this->subspaces = subspaces;
	this->included = included;

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		this->states[name] = states[name];
	};

	return;
};



template <typename T>
void Ising<T>::compute(){
	
	unsigned int N = this->system.N; // Number of sites
	unsigned int D = this->system.D; // Dimension of sites
	unsigned int n = this->system.n; // Number of states
	unsigned int s = this->system.s; // Number of unique states to process
	unsigned int q = this->system.q; // Number of states to process
	unsigned int j,k,l;
	std::string name;
	T value;
	typename tensor::Tensor<T>::vector_complex eigenvector;

	// Sort eigenvalues
	std::vector<int> argsort; // Sorting indices of eigenvalues
	std::string sorting = this->system.sorting; // Sorting algorithm of eigenvalues
	int axis = 1; // Axis of eigenvectors for sorting
	T eps = this->system.eps; // Floating point tolerance
	T tol = this->system.tol; // Tolerance of closeness of sorted eigenvalues
	T close = this->system.close; // Tolerance of closeness of sorted eigenvalues

	utils::check(this->eigenvalues,eps);
	utils::check(this->eigenvectors,eps);
	linalg::argsort(this->eigenvalues,argsort,sorting);
	linalg::permute(this->eigenvalues,argsort);
	linalg::permute(this->eigenvectors,argsort,axis);

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices; // vector of vectors of grouped indices of equal eigenvectors
	std::vector<unsigned int> size; // size of each group of equal eigenvectors

	q = std::min(std::min((unsigned int)(this->eigenvalues.size()),(unsigned int)(this->eigenvectors.cols())),q);
	for (i=0;i<q;i++){

		if ((size.size() == 1) and (this->system.parameters["h"] == 0)){
			tol = close;
		}
		else {
			tol = this->system.tol;
		};
		if ((i==0) or (not utils::close(this->eigenvalues(i),this->eigenvalues(indices.back().back()),tol))){
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
	std::map<std::string,typename tensor::Tensor<T>::matrix> state = this->state;
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		if ((name == "order") or (name == "energy") or (name == "gap")){
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		}
		else if ((name == "entanglement") or (name == "partition")){
			state[name] = tensor::Tensor<T>::matrix::Zero(s,N-1);
		}
		else {
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		};
	};


	for (k=0; k<s; k++){

		// Equal Superposition of degenerate eigenvectors in full space from subspace
		eigenvector = tensor::Tensor<T>::vector_complex::Zero(n);
		for (i=0;i<size[k];i++){
			if (this->subspaces.size() == n and false){
				eigenvector = this->eigenvectors.col(indices[k][i]);
			}
			else {
				for (j=0;j<this->subspaces.size();j++){
					eigenvector(this->subspaces[j]) += this->eigenvectors(j,indices[k][i]);
				};
			};
		};	
		eigenvector /= std::sqrt(size[k]);

		// Order
		name = "order";
		value = physics::expectation(eigenvector,this->states[name],eps)/N;
		state[name](k) = value;

		// Energy
		name = "energy";
		value = this->eigenvalues(indices[k]).sum()/size[k]/N;
		state[name](k) = value;

		// Gap
		name = "gap";
		value = (this->eigenvalues(indices[std::min(k+1,s-1)].front()) - this->eigenvalues(indices[k].front()))/N;
		state[name](k) = value;

		// Entanglement
		name = "entanglement";

		// #pragma omp parallel for private(j,l,value,eigenvector,eps) shared(k,name,state)
		for (j=0;j<state[name].cols();j++){
			l = pow(D,j+1);
			value = physics::entropy(eigenvector,l,eps);

			// #pragma omp critical
			state[name](k,j) = value;
		};
		
		// Partition
		name = "partition";

		for (j=0;j<state[name].cols();j++){
			state[name](k,j) = ((T)j+1)/((T)N);
		};

	};


	// Check
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		utils::check(state[name],eps);
		this->state[name] = state[name];
	};

	return;

};


// Constructor and Destructor
template <typename T>
Heisenberg<T>::Heisenberg(tensor::System<T> & system) : tensor::Tensor<T>(system){
	this->setup(system);
};

template <typename T>
Heisenberg<T>::~Heisenberg(){
};


template <typename T>
void Heisenberg<T>::setup(tensor::System<T> & system){
	
	unsigned int i;
	typename std::map<std::string,std::vector<T>>::iterator iterator;	
	unsigned int size = this->size;
	unsigned int nnz = this->nnz;
	unsigned int N = system.N;
	unsigned int D = system.D;
	unsigned int s = system.s;
	std::string name;

	for (i=0;i<system.state.size();i++){
		name = system.state[i];
		this->states[name];
		this->state[name];
	};

	for (iterator=system.symmetries.begin();iterator!=system.symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			if (system.symmetries[name].size() == 0){
				system.symmetries[name].resize(D*N+1);
				std::iota(system.symmetries[name].begin(),system.symmetries[name].end(),-1.0*N);
			};
		};
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

};



template <typename T>
void Heisenberg<T>::set(){

	unsigned int i,j,k,s,t;
	std::string name;
	T value;

	std::default_random_engine generator;
	std::uniform_real_distribution<T> distribution(0.0,1.0);

	unsigned int size;
	typename std::map<std::string,std::vector<T>>::iterator iterator;
	std::vector<unsigned int> subspaces;
	std::map<unsigned int,int> included;	
	T symmetry;
	std::map<std::string,std::vector<T>> symmetries = this->system.symmetries;

	unsigned int n = this->system.n;
	unsigned int N = this->system.N;
	unsigned int nnz = this->system.nnz;
	std::map<std::string,T> parameters = this->system.parameters;
	std::map<std::string,typename tensor::Tensor<T>::vector> states = this->states;

	for (iterator=symmetries.begin();iterator!=symmetries.end();iterator++){
		name = iterator->first;
		if (name == "order"){
			// #pragma omp parallel for private(i,symmetry) shared(subspaces,included)
			for (i=0;i<n;i++){
				symmetry = physics::spincount(i,N);
				if (utils::isin(symmetries[name],symmetry)){
					
					// #pragma omp critical
					subspaces.push_back(i);

					// #pragma omp critical
					included[i] = subspaces.size()-1;
				}
				else{
					// #pragma omp critical
					included[i] = -1;
				};
			};		
		};		
	};

	size = subspaces.size();
	nnz = std::min(nnz,size*size);

	typename tensor::Tensor<T>::type data(size,size);
	data.reserve(nnz);

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		if ((name == "order")){
			states[name] = tensor::Tensor<T>::vector::Zero(size);
		}
		else if ((name == "energy") or (name == "gap")){
			states[name];
		}
		else if ((name == "entanglement") or (name == "partition")){
			states[name];
		}
		else {
			states[name];
		};		
	};

	typedef typename tensor::Tensor<T>::index index;
	std::vector<index> indexes;


	this->parallel();
	#pragma omp parallel for private(i,j,k,s,t,name,value) shared(states,indexes,subspaces,included)
	for (s=0;s<size;s++){

		// Index
		i = subspaces[s];

		// States
		// for (i=0;i<this->system.state.size();i++){
		// 	name = this->system.state[i];
		// 	if ((name == "order")){
		// 		value = physics::spincount(i,N);
		// 		states[name](s) = value;
		// 	}
		// };

		// Order
		name = "order";
		value = physics::spincount(i,N);
		states[name](s) = value;

		for (k=0; k<N; k++){

			// ZZ Term
			j = i;
			t = s;
			value = -parameters["J"]*physics::spin(i,(k)%N)/2*physics::spin(j,(k+1)%N)/2;

			// std::cout << "ZZ value " << s << " " << t << " " << value << std::endl;

			#pragma omp critical
			indexes.push_back(index(s,t,value));


			// Z Term
			j = i;
			t = s;
			value = -(2*distribution(generator)-1)*parameters["h"]*physics::spin(i,(k)%N)/2;

			#pragma omp critical
			indexes.push_back(index(s,t,value));	

			// #pragma omp critical
			// std::cout << "h value " << s << " " << t << " " << value << std::endl;


			// X Term
			// j = physics::spinflip(i,(k)%N);
			// if (included[j] != -1){
			// 	t = included[j];
			// 	value = -parameters["g"];

			// 	#pragma omp critical
			// 	indexes.push_back(index(s,t,value));
			// };		

			// XX + YY Term
			if (physics::spin(i,(k)%N) != physics::spin(i,(k+1)%N)){
				j = physics::spinswap(i,(k)%N,(k+1)%N);
				if (included[j] != -1){
					t = included[j];
					value = -parameters["U"]/2;

					#pragma omp critical
					indexes.push_back(index(s,t,value));

					// std::cout << "XY value " << s << " " << t << " " << value << std::endl;

					// std::cout << s << " " << i << " " << k << " " << j << " " << t << std::endl;
					// std::cout << s << " " << i << " " << k << " " << j << " " << t << std::endl;

				}
				else {
					// std::cout << s << " " << i << " " << k << " " << j << std::endl;
				};
			};

		};
		// std::cout << std::endl;


	};

	data.setFromTriplets(indexes.begin(),indexes.end());

	this->size = size;
	this->nnz = nnz;
	this->data = data;

	this->subspaces = subspaces;
	this->included = included;

	for (i=0;i<this->system.state.size();i++){
		name = this->system.state[i];
		this->states[name] = states[name];
	};

	return;
};



template <typename T>
void Heisenberg<T>::compute(){
	
	unsigned int N = this->system.N; // Number of sites
	unsigned int D = this->system.D; // Dimension of sites
	unsigned int n = this->system.n; // Number of states
	unsigned int s = this->system.s; // Number of unique states to process
	unsigned int q = this->system.q; // Number of states to process
	unsigned int j,k,l;
	std::string name;
	T value;
	typename tensor::Tensor<T>::vector_complex eigenvector;

	// Sort eigenvalues
	std::vector<int> argsort; // Sorting indices of eigenvalues
	std::string sorting = this->system.sorting; // Sorting algorithm of eigenvalues
	int axis = 1; // Axis of eigenvectors for sorting
	T eps = this->system.eps; // Floating point tolerance
	T tol = this->system.tol; // Tolerance of closeness of sorted eigenvalues
	T close = this->system.close; // Tolerance of closeness of sorted eigenvalues

	utils::check(this->eigenvalues,eps);
	utils::check(this->eigenvectors,eps);
	linalg::argsort(this->eigenvalues,argsort,sorting);
	linalg::permute(this->eigenvalues,argsort);
	linalg::permute(this->eigenvectors,argsort,axis);

	// Unique sorted and grouped eigenvalue indices
	unsigned int i;
	std::vector<std::vector<unsigned int>> indices; // vector of vectors of grouped indices of equal eigenvectors
	std::vector<unsigned int> size; // size of each group of equal eigenvectors

	q = std::min(std::min((unsigned int)(this->eigenvalues.size()),(unsigned int)(this->eigenvectors.cols())),q);
	for (i=0;i<q;i++){

		if ((size.size() == 1) and (this->system.parameters["h"] == 0)){
			tol = close;
		}
		else {
			tol = this->system.tol;
		};
		if ((i==0) or (not utils::close(this->eigenvalues(i),this->eigenvalues(indices.back().back()),tol))){
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
	std::map<std::string,typename tensor::Tensor<T>::matrix> state = this->state;
	for (k=0;k<this->system.state.size();k++){
		name = this->system.state[k];
		if ((name == "order") or (name == "energy") or (name == "gap")){
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		}
		else if ((name == "entanglement") or (name == "partition")){
			state[name] = tensor::Tensor<T>::matrix::Zero(s,N-1);
		}
		else {
			state[name] = tensor::Tensor<T>::vector::Zero(s);
		};
	};


	for (k=0; k<s; k++){

		// Equal Superposition of degenerate eigenvectors in full space from subspace
		eigenvector = tensor::Tensor<T>::vector_complex::Zero(n);
		for (i=0;i<size[k];i++){
			if (this->subspaces.size() == n and false){
				eigenvector = this->eigenvectors.col(indices[k][i]);
			}
			else {
				for (j=0;j<this->subspaces.size();j++){
					eigenvector(this->subspaces[j]) += this->eigenvectors(j,indices[k][i]);
				};
			};
		};	
		eigenvector /= std::sqrt(size[k]);

		// Order
		name = "order";
		value = physics::expectation(eigenvector,this->states[name],eps)/N;
		state[name](k) = value;

		// Energy
		name = "energy";
		value = this->eigenvalues(indices[k]).sum()/size[k]/N;
		state[name](k) = value;

		// Gap
		name = "gap";
		value = (this->eigenvalues(indices[std::min(k+1,s-1)].front()) - this->eigenvalues(indices[k].front()))/N;
		state[name](k) = value;

		// Entanglement
		name = "entanglement";

		// #pragma omp parallel for private(j,l,value,eigenvector,eps) shared(k,name,state)
		for (j=0;j<state[name].cols();j++){
			l = pow(D,j+1);
			value = physics::entropy(eigenvector,l,eps);

			// #pragma omp critical
			state[name](k,j) = value;
		};
		
		// Partition
		name = "partition";

		for (j=0;j<state[name].cols();j++){
			state[name](k,j) = ((T)j+1)/((T)N);
		};

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

template class Ising<double>;
template class Ising<float>;

template class Heisenberg<double>;
template class Heisenberg<float>;

};