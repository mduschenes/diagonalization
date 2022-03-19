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

	int N = 2;
	int D = 2;
	int d = 1;
	int K = 3;
	T J = 1;
	T h = 1;
	T U = 0;

	argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){K = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){J = std::atof(argv[argn-1]);};
	argn++;if (argc >= argn){h = std::atof(argv[argn-1]);};
	argn++;if (argc >= argn){U = std::atof(argv[argn-1]);};

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
	system.parameters = {J,h,U}; // Parameters of length K

	hamiltonian::Hamiltonian<T> H(system);

	H.set();
	H.eig();
	H.dump();


	// Iterations

	// hamiltonian::Hamiltonian<T> H;
	// tensor::System<T> system;

	// unsigned int _N_ = 8;
	// std::vector<int> _N = {2,4,6,8,10,12,14,16};
	// // for (i=0;i<_N_;i++){_N.push_back(2+2*i);};	

	// unsigned int _h_ = 100;
	// std::vector<T> _h;
	// for (i=0;i<_h_;i++){_h.push_back(2+2*i);};

	// unsigned int _J_ = 1;
	// std::vector<T> _J = {1};
	// // for (i=0;i<_J_;i++){_J.push_back(1);};

	// unsigned int _U_ = 1;
	// std::vector<T> _U = {0};
	// // for (i=0;i<_U_;i++){_U.push_back(2+2*i);};

	// for (auto&& [i, N,J,h,U] : itertools::enumerate(itertools::product(_N,_J,_h,_U))){
	// 	system.N = N;
	// 	system.D = D;
	// 	system.d = d;
	// 	system.n = pow(D,N);
	// 	system.K = K;
	// 	system.size = pow(D,N); 
	// 	system.dim = 2; 
	// 	system.path = "data/data.hdf5"; // path name
	// 	system.group = std::format("data_{}",i); // group name
	// 	system.name = "data"; // dataset name
	// 	system.data = "data"; // data name
	// 	system.metadata = "metadata"; // metadata name
	// 	system.parameters = {J,h,U}; // Parameters of length K

	// 	H = hamiltonian::Hamiltonian<T>(system);
	
	// 	H.set();
	// 	H.eig();
	// 	H.dump();

	// };


	return 0;
};

