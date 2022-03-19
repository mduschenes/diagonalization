#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>
#include <random>
#include <typeinfo>

#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 

#include <LBFGS.h>

#include <itertools.hpp>
	
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
	int k; // Number of parameters	
	int s; // Number of states
	int size; // Data size
	int dim = 2; // Data dimension
	std::vector<type> parameters; // parameters	
	std::string path = "data.hdf5"; // path name
	std::string group = "data"; // group name
	std::string name = "data"; // object name
	std::string data = "data"; // data name
	std::string metadata = "metadata"; // metadata name
	std::string state = "state"; // state name
	std::vector<std::string> names = {"N","D","d","n","k","s"}; // variable names
	std::vector<std::string> params; // parameter names
	std::vector<std::string> states; // state names
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
		typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;
		typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix;
		typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> matrix_complex;
		typedef Eigen::Vector<T, Eigen::Dynamic> vector;
		typedef Eigen::Vector<std::complex<T>, Eigen::Dynamic> vector_complex;

		// Data
		type data;

		// State
		vector state;
		matrix states;

		// Setup
		void setup(tensor::System<T> & system);

		// Save and Load
		void dump();
		void load();

		// Print
		void print();

		// Random Set
		void rand();

		// Solve
		Eigen::SelfAdjointEigenSolver<type> solver;
		vector eigenvalues;
		matrix_complex eigenvectors;
		void eig();

	private:

};


};

#endif
