#ifndef _TENSOR_
#define _TENSOR_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector> 
#include <complex>
#include <cmath>
#include <map>


#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues> 

#include <Eigen/Core>
#include <Spectra/SymEigsSolver.h>
// #include <Spectra/MatOp/DenseSymMatProd.h>
// #include <Spectra/MatOp/SparseSymMatProd.h>
// #include <unsupported/Eigen/ArpackSupport>
// #include <Eigen/SparseCholesky>
// #include <ArpackSelfAdjointEigenSolver>
// #include <LBFGS.h>


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
	int z; // coordination number
	int k; // Number of parameters	
	int s; // Number of unique states
	int q; // Number of states
	int size; // Data size
	int dim = 2; // Data dimension
	type eps = 1e-14; // floating point tolerance
	std::string path = "data.hdf5"; // path name
	std::string group = "data"; // group name
	std::string name = "data"; // object name
	std::string data = "data"; // data name
	std::string metadata = "metadata"; // metadata name
	std::map<std::string,type> parameters; // parameters
	std::vector<std::string> state; // state names
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
		// typedef Eigen::SparseMatrix<T> type;
		typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type_complex;		
		typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix;
		typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> matrix_complex;
		typedef Eigen::Vector<T, Eigen::Dynamic> vector;
		typedef Eigen::Vector<std::complex<T>, Eigen::Dynamic> vector_complex;
		
		// typedef Eigen::SparseMatrix<T> SparseMat;
		// typedef Eigen::SimplicialLDLT<SparseMat> SparseChol;
		// typedef Eigen::ArpackSelfAdjointEigenSolver<SparseMat, SparseChol> Arpack;
		// Arpack arpack;

		// Data
		type data;

		// State
		std::map<std::string,vector> states;
		std::map<std::string,vector> state;

		// Setup
		void setup(tensor::System<T> & system);

		// Save and Load
		void dump();
		void load();

		// Print
		void print();

		// Solve
		
		// typedef Spectra::SymEigsSolver<Spectra::DenseSymMatProd<T>> solver;
		// typedef Spectra::DenseSymMatProd<T> op;

		// typedef Spectra::SymEigsSolver<Spectra::SparseSymMatProd<T>> solver;
		// typedef Spectra::SparseSymMatProd<T> op;

		// Spectra::SortRule sort = Spectra::SortRule::LargestMagn;

		typedef Eigen::SelfAdjointEigenSolver<type> solver;

		vector eigenvalues;
		matrix_complex eigenvectors;
		void eig();

	private:

};


};

#endif
