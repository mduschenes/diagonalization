#include <iostream>
#include <cstddef>
#include <vector> 
#include <complex> 

#include "hamiltonian.hpp"


int main(int argc, char *argv[]){

	mkl_set_num_threads(NUM_THREADS);
	omp_set_num_threads(NUM_THREADS);

	int argn = 1;

	int N = 2;
	int D = 2;
	int d = 1;
	int K = 3;
	typedef double T;

	argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){K = std::atoi(argv[argn-1]);};

	tensor::System<T> system;
	system.N = N;
	system.D = D;
	system.d = d;
	system.n = pow(D,N);
	system.K = K;
	system.size = pow(D,N); 
	system.dim = 2; 
	system.path = "data/data.hdf5"; // path name
	system.group = "data"; // group name
	system.name = "data"; // dataset name
	system.data = "data"; // data name
	system.metadata = "metadata"; // metadata name
	system.parameters = {1,0.5,1}; // Parameters of length K


	hamiltonian::Hamiltonian<T> H(system);

	H.set();
	H.eig();
	H.dump();

	return 0;
};

