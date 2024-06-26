#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>
#include <map>
#include <thread>
#include <variant>
#include <random>

#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <itertools.hpp>

#include "io.hpp"	
#include "utils.hpp"
#include "linalg.hpp"

namespace tensor {


// System variables
template <class type>
struct System {
	unsigned int N; // number of sites
	unsigned int D; // dimension of sites
	unsigned int d; // spatial dimensions
	unsigned int n; // Total dimension of space
	unsigned int z; // coordination number
	unsigned int k; // Number of parameters	
	std::string model = "hamiltonian"; // Model
	std::string space = "spin"; // Local space
	std::string lattice = "square"; // Lattice type
	unsigned int s; // Number of unique states
	unsigned int q; // Number of states
	std::string sigma; // State shift parameter
	type shift = 0; // State shift value
	type scale = 1; // State scale value	
	std::string sorting; // Sorting for states
	unsigned int size; // Data size
	unsigned int dim = 2; // Data dimension
	type eps = 1e-14; // Floating point tolerance
	type tol = 1e-14; // State degeneracy tolerance
	type close = 1e-14; // Equal State degeneracy tolerance
	unsigned int nnz = 0; // number of data elements
	bool sparse = true; // sparsity of data
	std::string path = "data.hdf5"; // path name
	std::string group = "data"; // group name
	std::string name = "data"; // object name
	std::string data = "data"; // data name
	std::string metadata = "metadata"; // metadata name
	std::vector<std::string> state; // state names
	std::map<std::string,type> parameters; // parameters
	std::map<std::string,std::vector<type>> symmetries; // symmetries	
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
		unsigned int size;
		unsigned int dim;
		unsigned int nnz;
		bool sparse;

		// Type
		// typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;				
		typedef Eigen::SparseMatrix<T,Eigen::ColMajor> type;
		typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix;
		typedef Eigen::Vector<T, Eigen::Dynamic> vector;

		// typedef Eigen::SparseMatrix<T,Eigen::ColMajor> type_complex;
		// typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix_complex;
		// typedef Eigen::Vector<T, Eigen::Dynamic> vector_complex;

		typedef Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> type_complex;
		typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> matrix_complex;
		typedef Eigen::Vector<std::complex<T>, Eigen::Dynamic> vector_complex;

		typedef Eigen::Triplet<T,unsigned int> index;

		// Parallel
		void parallel(){Eigen::initParallel();};

		// Data
		type data;

		// State
		std::map<std::string,vector> states;
		std::map<std::string,matrix> state;
		std::vector<unsigned int> subspaces;
		std::map<unsigned int,int> included;

		// Save and Load
		void dump();
		void load();

		// Print
		void print();

		// Solve
		vector eigenvalues;
		matrix_complex eigenvectors;
		void eig();

};


};

#endif
