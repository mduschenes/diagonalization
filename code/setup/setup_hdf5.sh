#!/bin/bash

cwd=$(pwd)

HDF5_VER_MAJOR=1.12
HDF5_VER_MINOR=1
HDF5_VER=${HDF5_VER_MAJOR}.${HDF5_VER_MINOR}

INSTALL_DIR="/usr/local/include"

HDF5_RELEASE_URL="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${HDF5_VER_MAJOR}/hdf5-${HDF5_VER}/src/hdf5-${HDF5_VER}.tar.gz"

TEMP_DIR="$(mktemp -d)"


cd ${TEMP_DIR}
mkdir ${HDF5_VER}
wget ${HDF5_RELEASE_URL} -O ${TEMP_DIR}/${HDF5_VER}.tar.gz
tar -xvzf ${HDF5_VER}.tar.gz -C ./${HDF5_VER} --strip-components=1

cd ${HDF5_VER}
bash ./configure --prefix=/usr/local --enable-cxx 
make -j5
# make check
sudo checkinstall make install
sudo make check-install

cd $cwd