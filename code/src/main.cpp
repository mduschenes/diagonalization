#include <iostream>
#include <cstddef>
#include <vector> 
#include <complex> 

#include "hamiltonian.hpp"
#include "utils.hpp"



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
	unsigned int i,j;
	bool multiple;
	using types = std::variant<T,unsigned int>;
	unsigned int size = 0;
	std::vector<std::string> names;
	std::map<std::string,unsigned int> sizes;
	std::map<std::string,types> variable;
	std::map<std::string,std::vector<types>> variables;

	if (argc > 1){
		argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){k = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){s = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){q = std::atoi(argv[argn-1]);};
		argn++;if (argc >= argn){J = std::atof(argv[argn-1]);};
		argn++;if (argc >= argn){h = std::atof(argv[argn-1]);};
		argn++;if (argc >= argn){U = std::atof(argv[argn-1]);};
		
		names.push_back("N");	
		sizes[names.back()] = 1;
		variables[names.back()].push_back(N);

		names.push_back("J");
		sizes[names.back()] = 1;
		variables[names.back()].push_back(J);

		names.push_back("h");
		sizes[names.back()] = 1;
		variables[names.back()].push_back(h);

		names.push_back("U");
		sizes[names.back()] = 1;
		variables[names.back()].push_back(U);

	}
	else {

		names.push_back("N");
		sizes[names.back()] = 6;
		variables[names.back()].insert(variables[names.back()].end(),{
			(unsigned int)4,
			(unsigned int)6,
			(unsigned int)8,
			(unsigned int)10,
			(unsigned int)12,
			(unsigned int)14,
			(unsigned int)16,
			(unsigned int)18,
			(unsigned int)20,
		});

		names.push_back("J");
		sizes[names.back()] = 1;
		variables[names.back()].insert(variables[names.back()].end(),{(T)1});

		names.push_back("h");
		sizes[names.back()] = 64;
		for (j=0;j<sizes[names.back()];j++){variables[names.back()].push_back((T)2*(j+1)/(sizes[names.back()]-1+1));};

		names.push_back("U");
		sizes[names.back()] = 1;
		variables[names.back()].insert(variables[names.back()].end(),{(T)0});

	};

	size = variables.size();
	multiple = false;
	for (j=0;j<size;j++){sizes[names[j]] = variables[names[j]].size();};
	for (j=0;j<size;j++){multiple |= sizes[names[j]]>1;};

	for (auto&& [i,iterable] : iter::enumerate(iter::product(variables[names[0]],variables[names[1]],variables[names[2]],variables[names[3]]))){

		// assign(iterable,variable,names);
		variable[names[0]] = std::get<0>(iterable); variable[names[1]] = std::get<1>(iterable);
		variable[names[2]] = std::get<2>(iterable); variable[names[3]] = std::get<3>(iterable);

		N = std::get<unsigned int>(variable["N"]); J = std::get<T>(variable["J"]);
		h = std::get<T>(variable["h"]); U = std::get<T>(variable["U"]);

		tensor::System<T> system;
		system.N = N; // number of sites
		system.D = D; // local site dimension
		system.d = d; // spatial dimension
		system.n = pow(system.D,system.N); // system size
		system.z = 2*d; // coordination number
		system.k = k; // number of parameters
		system.space = "spin"; // Local site space
		system.lattice = "square"; // Lattice type
		system.s = 3; // number of unique eigenvalues to consider
		system.q = 4;//2*pow(system.N,2)+4 + system.N; //std::max(q,int(pow(system.D,system.N))); // number of eigenvalues to consider
		system.sigma = "LA"; // State shift parameter
		system.scale = 100; // solver parameter
		system.sorting = "<="; // Sorting for states
		system.size = pow(system.D,system.N); // data size
		system.dim = 2; // data dimension
		system.eps = 1e-6; // floating point tolerance
		system.sparse = true; // sparsity of data
		system.nnz = 2*pow(system.D,system.N)*system.N; // number of data elements
		system.path = "data/data.hdf5"; // path name
		if (multiple) {system.group = "data_" + std::to_string(i);} else {system.group = "data";}; // group name
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

