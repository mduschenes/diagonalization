#!/bin/bash

# rm build -rf

# mkdir build

# cd build

export FFLAGS="-DMKL -I/opt/intel/mkl/include"
export FCFLAGS="-DMKL -I/opt/intel/mkl/include"
export LIBS="-Wl,--no-as-needed -L/opt/intel/mkl/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5 -fopenmp -lpthread -lm -ldl"
export INTERFACE64=0


# cmake -D EXAMPLES=ON -D MPI=ON -D BUILD_SHARED_LIBS=ON ..
sh bootstrap	
sudo FFLAGS="-DMKL_LP64 -I/opt/intel/mkl/include" FCFLAGS="-DMKL_LP64 -I/opt/intel/mkl/include" LIBS="-Wl,--no-as-needed -L/opt/intel/mkl/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5 -fopenmp -lpthread -lm -ldl" ./configure --with-blas=mkl_intel_lp64 --with-lapack=mkl_intel_lp64

make
make install
make all check