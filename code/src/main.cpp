#include <iostream>
#include <cstddef>
#include <vector> 
#include <complex> 

#include "hamiltonian.hpp"


int main(int argc, char *argv[]){

	mkl_set_num_threads(NUM_THREADS);
	omp_set_num_threads(NUM_THREADS);

	int argn = 1;

	typedef double T;

	unsigned int N = 4;
	unsigned int D = 2;
	unsigned int d = 1;
	unsigned int k = 3;
	unsigned int s = 2;
	unsigned int q = 2*N+4;
	T J = 1;
	T h = 1;
	T U = 0;

	// Iterations
	unsigned int i;

	if (argc > 0){
		argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){k = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){s = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){q = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){J = std::atof(argv[argn-1]);};
		argn++;if (argc >= argn){h = std::atof(argv[argn-1]);};
		argn++;if (argc >= argn){U = std::atof(argv[argn-1]);};
		

		unsigned int _N_ = 5;
		std::vector<unsigned int> _N = {N};
		// for (i=0;i<_N_;i++){_N.push_back(2+2*i);};	
		_N_ = _N.size();

		unsigned int _J_ = 1;
		std::vector<T> _J = {J};
		// for (i=0;i<_J_;i++){_J.push_back(1);};
		_J_ = _J.size();

		unsigned int _h_ = 1;
		std::vector<T> _h = {h};
		// for (i=0;i<_h_;i++){_h.push_back(2.0*(i+1)/(_h_-1+1));};
		_h_ = _h.size();

		unsigned int _U_ = 1;
		std::vector<T> _U = {U};
		// for (i=0;i<_U_;i++){_U.push_back(2+2*i);};
		_U_ = _U.size();

	}
	else {

		unsigned int _N_ = 5;
		std::vector<unsigned int> _N = {6,8,10,12,14};
		// for (i=0;i<_N_;i++){_N.push_back(2+2*i);};	
		_N_ = _N.size();

		unsigned int _J_ = 1;
		std::vector<T> _J = {1};
		// for (i=0;i<_J_;i++){_J.push_back(1);};
		_J_ = _J.size();

		unsigned int _h_ = 20;
		std::vector<T> _h;
		for (i=0;i<_h_;i++){_h.push_back(2.0*(i+1)/(_h_-1+1));};
		_h_ = _h.size();

		unsigned int _U_ = 1;
		std::vector<T> _U = {0};
		// for (i=0;i<_U_;i++){_U.push_back(2+2*i);};
		_U_ = _U.size();

	};

	i = -1;
	for (auto&& [N,J,h,U] : iter::product(_N,_J,_h,_U)){

		i ++;

		tensor::System<T> system;
		system.N = N; // number of sites
		system.D = D; // local site dimension
		system.d = d; // spatial dimension
		system.n = pow(D,N); // system size
		system.z = 2*d; // coordination number
		system.k = k; // number of parameters
		system.space = "spin"; // Local site space
		system.lattice = "square"; // Lattice type
		system.s = 4; // number of unique eigenvalues to consider
		system.q = 4;//2*pow(N,2)+4 + N; //std::max(q,int(pow(D,N))); // number of eigenvalues to consider
		system.sigma = "LA"; // State shift parameter
		system.sorting = "<="; // Sorting for states
		system.tol = 100; // solver parameter
		system.size = pow(D,N); // data size
		system.dim = 2; // data dimension
		system.eps = 1e-13; // floating point tolerance
		system.sparse = false; // sparsity of data
		system.nnz = 2*pow(D,N)*N; // number of data elements
		system.path = "data/data.hdf5"; // path name
		system.group = "data_" + std::to_string(i); // group name
		system.name = "data"; // dataset name
		system.data = "data"; // data name
		system.metadata = "metadata"; // metadata name
		system.state = {
			"order","energy","gap","entanglement",
			}; // states
		system.parameters = {{"J",J},{"h",h},{"U",U}}; // Parameters of length k

		hamiltonian::Hamiltonian<T> H(system);

		H.print();
		H.set();
		std::cout << "set" << std::endl;	
		H.eig();
		std::cout << "eig" << std::endl;	
		H.compute();
		std::cout << "compute" << std::endl;			
		H.dump();
		std::cout << "dump" << std::endl;	
		std::cout << std::endl;	


	};

	return 0;
};

