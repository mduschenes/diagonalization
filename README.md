# diagonalization

Fast exact diagonalization of up to N=26 arbitrary qubit Hamiltonians

Encode symmetries in Hamiltonians and use efficient bitwise operations

Fast calculation in C++ and post-processing in Python of observables 
and quantities of interest

Existing models: Transverse Field Ising Model, Anisotropic Heisenberg Model, XY Model, Random-Field Heisenberg Model

Existing observables: Enerty, Magnetic order parameter, Entanglement Entropy

Backend with sparse Eigen and ARPACK with MKL BLAS and LAPACK, and OpenMP for (Arnoldi-based) eigenvalue and singular value calculations

Data saved with HDF5

# Install
. setup/install.sh

# Directories
paper: Analyis of results

code: Source code

# Run
All calculations and post-processing through makefile commands 

All: make

Calculations: make run

Post-processing: make plot

Specify calculations settings in src/main.cpp

Specify post-processing settings in src/main.py
