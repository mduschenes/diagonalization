#include "tensor.hpp"
#include "utils.hpp"

#define NUM_THREADS 7


int main(int argc, char *argv[]){

    mkl_set_num_threads(NUM_THREADS);
    omp_set_num_threads(NUM_THREADS);
	
    int argn = 1;

    int N = 2;
    int D = 2;
    int d = 1;
    int K = 3;
    std::vector<double> theta(K,0.0);

    // argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){K = std::atoi(argv[argn-1]);};

    tensor::System<double> system;
    system.N = N;
    system.D = D;
    system.d = d;
    system.n = pow(D,N);
    system.K = K;
    system.size = pow(D,N); 
    system.dim = 2; 
    system.data = "data"; // data name
    system.metadata = "metadata"; // metadata name
    system.theta = theta;

 
	tensor::Tensor<double> A(system);

	std::vector<double> params = theta;
	A.set(params);

    
    std::string path = "data/data.hdf5";
	// A.print();
    // A.load(path);
    A.dump(path);
    // A.print();

    A.eig();

	return 0;
};

