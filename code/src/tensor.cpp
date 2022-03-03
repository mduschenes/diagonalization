#include "tensor.hpp"
#include "utils.hpp"

namespace tensor {



// Constructor and Destructor
template <class T>
Operator<T>::Operator(tensor::System<T> & system){
	this->set(system);
};

template <class T>
Operator<T>::~Operator(){
};


// Set system
template <class T>
void Operator<T>::set(tensor::System<T> & system){


	// Theta
	this->system.theta.clear();
	for (int i=0;i<system.K;i++){

	}

	// if (this->exists == false){
	// 	this->system = system;
	// };

	this->system = system;
};

template <class T>
void Operator<T>::assemble(std::vector<T> theta){
	int N = this->system.N;
	int D = this->system.D;
	int Q = this->system.Q;

	T default_ = 0;
	
	Eigen::MatrixXd data(Q,Q);
	for (int i=0;i<(pow(Q,2));i++){
		data(i) =  default_;
	};

	this->data = data;
};


template <class T>
void Operator<T>::print(){
	std::cout << "N = " << this->system.N << std::endl;
	std::cout << "D = " << this->system.D << std::endl;
	std::cout << "d = " << this->system.d << std::endl;
	std::cout << "Q = " << this->system.Q << std::endl;
	std::cout << "K = " << this->system.K << std::endl;
	std::cout << "|O| = " << utils::norm(this->data,this->data) << std::endl;

};


template class Operator<double>;
template class System<double>;
template class Observables<double>;

template class Operator<int>;
template class System<int>;
template class Observables<int>;

};

