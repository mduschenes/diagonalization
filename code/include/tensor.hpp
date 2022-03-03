// #pragma once
#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <stdlib.h>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>

#include <Eigen/Dense>
#include <LBFGS.h>

namespace tensor {



// System variables
template <class type>
struct System {
	int N; // number of qubits
	int D; // dimension of qudits
	int d; // spatial dimensions
	int Q; // Total dimension of space
	int K; // Number of parameters
	std::vector<type> theta; // parameters
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



//  Class for operator
template <class T>
class Operator {
 
	public:
		//  Constructor and Destructor
		Operator(tensor::System<T> & system);
		~Operator();


		// System settings
		void set(tensor::System<T> & system);
		bool exists = false;
		tensor::System<T> system;

		// Print
		void print();

		// Assemble
		void assemble(std::vector<T> theta);

		// Data
		Eigen::MatrixXd data;

	private:

};


};

#endif
