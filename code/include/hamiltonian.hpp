#ifndef _HAMILTONIAN_
#define _HAMILTONIAN_

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
	
#include "tensor.hpp"
#include "utils.hpp"
#include "io.hpp"


namespace hamiltonian {

//  Class for Hamiltonian
template <typename T>
class Hamiltonian : public tensor::Tensor<T> {

	public:
		//  Constructor and Destructor
		Hamiltonian(tensor::System<T> & system);
		~Hamiltonian();


		// Set
		void set(std::vector<T> & theta);

	private:

};




};

#endif