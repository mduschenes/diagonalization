#include "utils.hpp"

namespace utils {

template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> commutator(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return A*B - B*A;
};

template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & A, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & B){
	return (A.transpose()*B).trace();
};

template <typename T>
struct H5TypeMap {
  static H5::DataType h5_type;
};

template<typename T>
H5::DataType H5Type(){
  return H5TypeMap<T>::h5_type;
};


template<typename T>
void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){
	const int dim = 2;
    hsize_t shape[dim] = {data.rows(), data.cols()};
    H5::DataType type = H5Type<T>();

	H5::H5File file(path.c_str(), H5::H5F_ACC_TRUNC);

 //    H5::DataSpace dataspace(dim, shape);
 //    H5::DataSet dataset = file.createDataSet(name.c_str(),type,dataspace);

 //    dataset.write(data.transpose().data(),type);

    return;
};

template<typename T>
void hdf5_to_eigen(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){
};




template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> commutator<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> commutator<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template <> H5::DataType H5TypeMap<double>::h5_type = H5T_NATIVE_DOUBLE;
template <> H5::DataType H5TypeMap<float>::h5_type = H5T_NATIVE_FLOAT;
template <> H5::DataType H5TypeMap<int>::h5_type = H5T_NATIVE_INT;


template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data);
template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data);
template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data);

};
