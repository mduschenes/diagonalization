// #pragma once
#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>

#define EIGEN_USE_MKL_ALL    
#include <Eigen/Dense>
#include <LBFGS.h>
#include <H5Cpp.h>


#include "utils.hpp"
#include "io.hpp"


namespace tensor {


// File Format
const static Eigen::IOFormat Format(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");


// System variables
template <class type>
struct System {
	int N; // number of qubits
	int D; // dimension of qudits
	int d; // spatial dimensions
	int n; // Total dimension of space
	int K; // Number of parameters	
	std::vector<type> theta; // parameters
	std::string data = "data";
	std::string metadata = "metadata";
	std::string ext = "csv";
	std::vector<std::string> strings = {"N","D","d","n","K"};
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
template <class T>
class Tensor {
 
	public:
		//  Constructor and Destructor
		Tensor(tensor::System<T> & system);
		~Tensor();


		// System settings
		void set(tensor::System<T> & system);
		bool exists = false;
		tensor::System<T> system;

		// Print
		void print();

		// Save and Load
		void dump(std::string path);
		void load(std::string path);

		// Assemble
		void assemble(std::vector<T> theta);

		// Data
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data;

	private:

};


};

#endif
