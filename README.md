# diagonalization

Fast exact diagonalization of up to N=26 arbitrary qubit Hamiltonians

Encode symmetries in Hamiltonians and use efficient bitwise operations

Fast calculation in C++ and post-processing in Python of observables 
and quantities of interest

Existing models: Transverse Field Ising Model, Anisotropic Heisenberg Model, XY Model, Random-Field Heisenberg Model

Existing observables: Enerty, Magnetic order parameter, Entanglement Entropy

Backend with sparse Eigen and ARPACK with MKL BLAS and LAPACK, and OpenMP for (Arnoldi-based) eigenvalue and singular value calculations

Data saved with HDF5

## Install
Under `setup`, please install the appropriate C++ and Python libaries with:
```sh
. setup/install.sh
```

## Setup
For model system settings (Temperature, couplings), under `build`, please modify the `main.cpp`
For specific system settings (Hamiltonian, observables), under `src`, please modify the `main.cpp`
For post-processing settings, under `src`, please modify `main.py`

## Run
Please use the makefile for all commands:
  - `make` : All calculations and processing
  - `make run` : All calculations
  - `make plot` : All processing
