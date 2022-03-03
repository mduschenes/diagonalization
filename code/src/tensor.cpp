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
	std::cout << std::endl;	
};

template <class T>
void Tensor<T>::dump(std::string path){

	io::io<T> io_data;
	std::string data;
	io_data.join(data,path,this->system.data,this->system.ext);

	std::cout << data << std::endl;

    std::ofstream file(data.c_str());
    file << this->data.format(tensor::Format);


	io::io<int> io_metadata;
	std::string metadata;
	io_metadata.join(metadata,path,this->system.metadata,this->system.ext);

	std::cout << metadata << std::endl;


	std::vector<std::string> header(this->system.strings);
	std::vector<int> values = {this->system.N,this->system.D,this->system.d,this->system.n,this->system.K};

    io_metadata.write(metadata,header,values);

};

template <class T>
void Tensor<T>::load(std::string path){
	
	io::io<T> io;

	std::string directory;
	io.join(directory,path,this->system.data,this->system.ext);

	std::vector<std::vector<T>> file;

	io.read(directory,file);

	int n = this->system.n;
    
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data(n,n);

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

