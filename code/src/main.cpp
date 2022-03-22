#include <iostream>
#include <cstddef>
#include <vector> 
#include <complex> 

#include "hamiltonian.hpp"


int main(int argc, char *argv[]){

	// Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> a;
	// Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> b(10,10);

	// b(5,5) = 3;

	// a = b;

	// a(5,5) = 9;

	// std::cout << b(5,5) << std::endl;

	// return 0;


	mkl_set_num_threads(NUM_THREADS);
	omp_set_num_threads(NUM_THREADS);

	int argn = 1;

	typedef double T;

	int N = 4;
	int D = 2;
	int d = 1;
	int k = 3;
	int s = 3;
	int q = 10;
	T J = 1;
	T h = 1;
	T U = 0;

	argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){k = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){s = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){q = std::atoi(argv[argn-1]);};
	argn++;if (argc >= argn){J = std::atof(argv[argn-1]);};
	argn++;if (argc >= argn){h = std::atof(argv[argn-1]);};
	argn++;if (argc >= argn){U = std::atof(argv[argn-1]);};

	tensor::System<T> system; // system attributes
	system.N = N; // number of sites
	system.D = D; // local site dimension
	system.d = d; // spatial dimension
	system.n = pow(D,N); // system size
	system.z = 2*d; // coordination number
	system.k = k; // number of parameters
	system.s = s; // number of unique eigenvalues to consider
	system.q = std::max(q,int(pow(D,N))); // number of eigenvalues to consider
	system.size = pow(D,N); // data size
	system.dim = 2; // data dimension
	system.eps = 1e-14; // floating point tolerance
	system.path = "data/data.hdf5"; // path name
	system.group = "data"; // group name
	system.name = "data"; // dataset name
	system.data = "data"; // data name
	system.metadata = "metadata"; // metadata name
	system.state = {
		"order","energy","gap","entanglement",
		}; // states
	system.parameters = {{"J",J},{"h",h},{"U",U}}; // Parameters of length k

	hamiltonian::Hamiltonian<T> H(system);

	H.set();
	std::cout << "set" << std::endl;
	H.eig();
	std::cout << "eig" << std::endl;	
	H.compute();
	std::cout << "compute" << std::endl;	
	H.dump();
	std::cout << "dump" << std::endl;	


	// // Iterations

	// // hamiltonian::Hamiltonian<T> H;
	// // tensor::System<T> system;

	// // unsigned int _N_ = 8;
	// // std::vector<int> _N = {2,4,6,8,10,12,14,16};
	// // // for (i=0;i<_N_;i++){_N.push_back(2+2*i);};	

	// // unsigned int _h_ = 100;
	// // std::vector<T> _h;
	// // for (i=0;i<_h_;i++){_h.push_back(2+2*i);};

	// // unsigned int _J_ = 1;
	// // std::vector<T> _J = {1};
	// // // for (i=0;i<_J_;i++){_J.push_back(1);};

	// // unsigned int _U_ = 1;
	// // std::vector<T> _U = {0};
	// // // for (i=0;i<_U_;i++){_U.push_back(2+2*i);};

	// // for (auto&& [i, N,J,h,U] : itertools::enumerate(itertools::product(_N,_J,_h,_U))){
	// // 	system.N = N;
	// // 	system.D = D;
	// // 	system.d = d;
	// // 	system.n = pow(D,N);
	// // 	system.k = k;
	// //	system.s = s;
	// // 	system.size = pow(D,N); 
	// // 	system.dim = 2; 
	// // 	system.path = "data/data.hdf5"; // path name
	// // 	system.group = std::format("data_{}",i); // group name
	// // 	system.name = "data"; // dataset name
	// // 	system.data = "data"; // data name
	// // 	system.metadata = "metadata"; // metadata name
	// //	system.state = "state"; // state name
	// //	system.states = {"order_0","order_1","order_2","energy_0","energy_1","energy_2","gap","entanglement"}; // state names
	// //	system.params = {"J","h","U"}; // parameter names
	// // 	system.parameters = {J,h,U}; // Parameters of length k

	// // 	H = hamiltonian::Hamiltonian<T>(system);
	
	// // 	H.set();
	// // 	H.eig();
	// // 	H.dump();

	// // };


	// return 0;
};

