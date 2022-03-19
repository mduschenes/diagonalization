#ifndef _HAMILTONIAN_
#define _HAMILTONIAN_

#include "tensor.hpp"
#include "io.hpp"
#include "utils.hpp"


namespace hamiltonian {

//  Class for Hamiltonian
template <typename T>
class Hamiltonian : public tensor::Tensor<T> {

	public:
		//  Constructor and Destructor
		Hamiltonian(tensor::System<T> & system);
		~Hamiltonian();

		// Set
		void set();

		// Compute
		void compute();

	private:

};




};

#endif