#!/bin/bash

# rm build -rf

# mkdir build

# cd build

export FFLAGS="-DMKL_LP64 -I${MKLROOT}/include -I/home/matt/files/uw/research/projects/diagonalization/code/lib/eigen"
export FCFLAGS="-DMKL_LP64 -I${MKLROOT}/include -I/home/matt/files/uw/research/projects/diagonalization/code/lib/eigen"
export CPPFLAGS="-DMKL_LP64 -I${MKLROOT}/include -I/home/matt/files/uw/research/projects/diagonalization/code/lib/eigen"
export CXXFLAGS="-DMKL_LP64 -I${MKLROOT}/include -I/home/matt/files/uw/research/projects/diagonalization/code/lib/eigen"
export LIBS="-Wl,--no-as-needed -Wl,--start-group -L${MKLROOT}/lib/intel64/libmkl_intel_lp64.a ${MKLROOT}/lib/intel64/libmkl_sequential.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group  -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5 -fopenmp -lpthread -lm -ldl -leigen"
export INTERFACE64=1

# cmake -D EXAMPLES=ON -D MPI=ON -D BUILD_SHARED_LIBS=ON ..
# sudo -E sh bootstrap	

sudo -E "FFLAGS=${FFLAGS} FCFLAGS=${FCFLAGS} LIBS=${LIBS} INTERFACE64=${INTERFACE64} CPPFLAGS=${CPPFLAGS} CXXFLAGS=${CXXFLAGS} ./configure --with-blas=mkl_intel_lp64 --with-lapack=mkl_intel_lp64 --enable-mpi "

make
make install
make all check