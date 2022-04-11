#ifndef _HAMILTONIAN_
#define _HAMILTONIAN_

#include "tensor.hpp"
#include "io.hpp"
#include "utils.hpp"
#include "linalg.hpp"


namespace hamiltonian {

//  Class for Hamiltonian
template <typename T>
class Hamiltonian : public tensor::Tensor<T> {

	public:
		//  Constructor and Destructor
		Hamiltonian(tensor::System<T> & system);
		~Hamiltonian();

		// Setup
		void setup(tensor::System<T> & system);

		// Set
		void set();

		// Compute
		void compute();

};

//  Class for Hamiltonian Ising
template <typename T>
class Ising : public tensor::Tensor<T> {

	public:
		//  Constructor and Destructor
		Ising(tensor::System<T> & system);
		~Ising();

		// Setup
		void setup(tensor::System<T> & system);

		// Set
		void set();

		// Compute
		void compute();

};



//  Class for Hamiltonian Heisenberg
template <typename T>
class Heisenberg : public tensor::Tensor<T> {

	public:
		//  Constructor and Destructor
		Heisenberg(tensor::System<T> & system);
		~Heisenberg();

		// Setup
		void setup(tensor::System<T> & system);

		// Set
		void set();

		// Compute
		void compute();

};


};

#endif