#include "tensor.hpp"
#include "utils.hpp"

int main(int argc, char *argv[]){
	
    int argn = 1;

    int N = 3;
    int D = 2;
    int d = 1;
    int K = 3;
    std::vector<int> theta(K,0.0);

    // argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
    argn++;if (argc >= argn){K = std::atoi(argv[argn-1]);};

    tensor::System<int> system;
    system.N = N;
    system.D = D;
    system.d = d;
    system.Q = pow(D,N);
    system.K = K;
    system.theta = theta;

 
	tensor::Operator<int> O(system);

	std::vector<int> params = theta;
	O.assemble(params);

	O.print();

	utils::norm(O.data,O.data);

	return 0;
};

