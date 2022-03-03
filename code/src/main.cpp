#include "tensor.hpp"
#include "utils.hpp"

int main(int argc, char *argv[]){
	
    int argn = 1;

    std::string path = "data.csv";
    std::string test = "test.csv";
    int N = 3;
    int D = 2;
    int d = 1;
    int K = 3;
    std::vector<double> theta(K,0.0);

    // argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){path = argv[argn-1];};
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
    system.theta = theta;

 
	tensor::Tensor<double> A(system);

	std::vector<double> params = theta;
	A.assemble(params);

	A.print();

    A.dump(path);

    A.load(test);
    // A.load(path);

    A.print();

	return 0;
};

