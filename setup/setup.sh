#!/bin/bash

cwd=$(pwd)

src=setup
pyversion=3.8
env=mkl
channel=intel

# Setup conda
source ~/miniconda3/etc/profile.d/conda.sh 
eval $(conda shell.bash hook)

# Clean conda
source ~/miniconda3/bin/activate base
conda remove --name ${env} --all
conda clean --all

# Create conda env
conda create --name ${env}
source ~/miniconda3/bin/activate ${env}
conda info --envs

# Install packages
cd ${src}
conda install --channel ${channel} --file requirements.py.txt
conda install --channel ${channel} --file requirements.mkl.txt
conda install --channel ${channel} --file requirements.npy.txt
conda install --file requirements.pkg.txt
pip install -r requirements.pip.txt
cd ${cwd}

# Install spack
git clone https://github.com/spack/spack.git lib/spack
source lib/spack/share/spack/setup-env.sh


# Install HDF5
./setup_hdf5.sh


# Test
src=test
tests=.
options=(-s)
cd ${src}
conda activate ${env}
pytest ${options[@]} ${tests}

cd ${cwd}
