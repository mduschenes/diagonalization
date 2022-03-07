#include <iostream>
#include <cstddef>
#include <vector> 

#include "utils.hpp"

#define EIGEN_USE_MKL_ALL   
#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 

int main(int argc, char *argv[]){

    int argn = 1;

    unsigned int n = 3;
    argn++;if (argc >= argn){n = std::atoi(argv[argn-1]);};

    unsigned int d = 2;
    unsigned int N = 1 << n;
    unsigned int m = n >> 1;

    unsigned int i,j,k;
    std::vector<unsigned int> z,x,y;
    x.resize(n);
    y.resize(n);
    z.resize(n);

    std::vector<double> theta = {9,};

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> a = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(N,N);

    #pragma omp parallel for private(i,j,k) shared(a)
    for (i=0;i<N;i++){
        if (utils::bitcount(i) == m) {
            for (k=0; k<n; k++){
                j = utils::flip(i,k);
                a(i,j) += theta[0]*utils::bit(j,k);
                // utils::bit(utils::phaseflip(i,k),k);
                // utils::bit(utils::phase(i,k),k);
            };
        };
    };

    for (i=0;i<N;i++){
        for (j=0;j<N;j++){
            std::cout << a(i,j) << " ";
        };
        std::cout << std::endl;
    };

    // std::cout << "n,N,m = " << n << ", " << N << ", " << m << std::endl;
    // for (j=0;j<N;j++){
    //     std::cout << "j = " << j;
    //     std::cout << " x,y,z = ";
    //     for (unsigned int i=0; i<n; i++){
    //         x[i] = utils::bit(utils::flip(j,i),i);
    //         y[i] = utils::bit(utils::phaseflip(j,i),i);
    //         z[i] = utils::bit(utils::phase(j,i),i);
    //         std::cout << x[i] << y[i] << z[i] << "|";
    //     };
    //     p = utils::polarization(j,n,m);
    //     std::cout << " p = " << p;
    //     std::cout << std::endl;
    // };

    return 0;
};


// #include "tensor.hpp"
// #include "utils.hpp"

// #define NUM_THREADS 7

// int main(int argc, char *argv[]){

//     mkl_set_num_threads(NUM_THREADS);
//     omp_set_num_threads(NUM_THREADS);
	
//     // argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
//     int argn = 1;

//     int N = 2;
//     int D = 2;
//     int d = 1;
//     int K = 3;
//     std::vector<double> theta(K,0.0);

//     // argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
//     argn++;if (argc >= argn){N = std::atoi(argv[argn-1]);};
//     argn++;if (argc >= argn){D = std::atoi(argv[argn-1]);};
//     argn++;if (argc >= argn){d = std::atoi(argv[argn-1]);};
//     argn++;if (argc >= argn){K = std::atoi(argv[argn-1]);};

//     tensor::System<double> system;
//     system.N = N;
//     system.D = D;
//     system.d = d;
//     system.n = pow(D,N);
//     system.K = K;
//     system.size = pow(D,N); 
//     system.dim = 2; 
//     system.data = "data"; // data name
//     system.metadata = "metadata"; // metadata name
//     system.theta = theta;

 
// 	tensor::Tensor<double> A(system);

// 	std::vector<double> params = theta;
// 	A.set(params);

    
//     std::string path = "data/data.hdf5";
// 	// A.print();
//     // A.load(path);
//     A.dump(path);
//     // A.print();

//     A.eig();

// 	return 0;
// };

