#include "tensor.hpp"

namespace tensor {



// Constructor and Destructor
template <class T>
Tensor<T>::Tensor(tensor::System<T> & system){
	this->set(system);
};

template <class T>
Tensor<T>::~Tensor(){
};


// Set system
template <class T>
void Tensor<T>::set(tensor::System<T> & system){




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
void Tensor<T>::assemble(std::vector<T> theta){
	int N = this->system.N;
	int D = this->system.D;
	int n = this->system.n;

	T default_ = 0;
	
	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data(n,n);
	for (int i=0;i<(pow(n,2));i++){
		data(i) =  default_;
	};

	this->data = data;
};


template <class T>
void Tensor<T>::print(){
	std::cout << "N = " << this->system.N << std::endl;
	std::cout << "D = " << this->system.D << std::endl;
	std::cout << "d = " << this->system.d << std::endl;
	std::cout << "n = " << this->system.n << std::endl;
	std::cout << "K = " << this->system.K << std::endl;
	std::cout << "|O| = " << utils::norm<T>(this->data,this->data) << std::endl;
	std::cout << "data = \n" << this->data << std::endl;

};

template <class T>
void Tensor<T>::dump(std::string path){

    std::ofstream file(path.c_str());
    file << this->data.format(tensor::Format);

};

template <class T>
void Tensor<T>::load(std::string path){
	
	int n = this->system.n;
    
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data(n,n);

	std::vector<std::vector<T>> file;

	this->io.read(path,file);

    for (int i = 0; i < n; i++){
    	for (int j = 0; j < n; j++){
        	data(i,j) = file[i][j];
    	};
    };

    this->data = data;

};

template class Tensor<double>;
template class System<double>;
template class Observables<double>;

template class Tensor<int>;
template class System<int>;
template class Observables<int>;

};

