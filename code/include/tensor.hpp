#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>
#include <random>

#define EIGEN_USE_MKL_ALL   
#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 

#include <LBFGS.h>
	
#include "utils.hpp"
#include "io.hpp"


namespace tensor {


// System variables
template <class type>
struct System {
	int N; // number of qubits
	int D; // dimension of qudits
	int d; // spatial dimensions
	int n; // Total dimension of space
	int K; // Number of parameters	
	int size; // Data size
	int dim = 2; // Data dimension
	std::vector<type> theta; // parameters	
	std::string group = "data"; // name
	std::string name = "data"; // name
	std::string data = "data"; // data name
	std::string metadata = "metadata"; // metadata name
	std::vector<std::string> strings = {"N","D","d","n","K"}; // settings names
};	

// Observables variables
template <class type>
struct Observables {
	std::vector<type> energy;
	std::vector<type> energy_mean;
	std::vector<type> energy_var;
	std::vector<type> order;
	std::vector<type> order_mean;
	std::vector<type> order_var;
};



//  Class for Tensor
template <typename T>
class Tensor {

	public:
		//  Constructor and Destructor
		Tensor(tensor::System<T> & system);
		~Tensor();

		// Settings
		tensor::System<T> system;

		// Size and Dimension
		const unsigned int size;
		const unsigned int dim;

		// Type
		typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Type;

		// Data
		Type data;

		// Setup
		void setup(tensor::System<T> & system);

		// Save and Load
		void dump(std::string path);
		void load(std::string path);

		// Print
		void print();

		// Random Set
		void rand(std::vector<T> & theta);

		// Solve
		Eigen::SelfAdjointEigenSolver<Tensor<T>::Type> solver;
		void eig();
		void eig(std::vector<T> & eigenvalues, Tensor<T>::Type & eigenvectors);

		// Convert
		// void convert(std::vector<std::vector<T>> & data);		

	private:

};


};

#endif
