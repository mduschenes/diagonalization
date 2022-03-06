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


bool ends_with(std::string const & value, std::string const & pattern){
    if (pattern.size() > value.size()) return false;
    return std::equal(pattern.rbegin(), pattern.rend(), value.rbegin());
};


template <typename T, typename U>
void cast(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> & other){
	
	other = data.template cast<U>();	

	return;
};



unsigned int bit(unsigned int x, unsigned int j){
	return (x >> j) & 1;
};

unsigned int phase(unsigned int x, unsigned int j){
	return bit(x,j)*x;
};

unsigned int flip(unsigned int x, unsigned int j){
	return x ^ (1 << j);
};

unsigned int phaseflip(unsigned int x, unsigned int j){
	return bit(x,j) * flip(x,j);
};

bool polarization(unsigned int x, unsigned int n, unsigned int m){
	unsigned int p = 0;
	unsigned int j = 0;
	while((p<=m)&&(j<n)){
		p += bit(x,j);
		j++;
	};
	return p==m;
};


// int ditphase(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x >> j) & u;
// };

// int ditraise(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x + 1)*((d - 1 - (x&(pow(d,j))))/d);
// };

// int ditlower(unsigned int & x, unsigned int & d,unsigned int & j, unsigned int & u){
// 	return (x - 1)*((d - 1 + (x&(pow(d,j))))/d);
// };




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
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
    hsize_t shape[dim] = {row,col};
    H5::DataType type = H5Type<T>();
    H5::DataType datatype(type);

	H5::H5File obj(path,H5F_ACC_TRUNC);

    H5::DataSpace dataspace(dim, shape);
    H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

    T * values = data.transpose().data();

    dataset.write(values,datatype);

    return;
};


template<typename T>
void vector_to_hdf5(std::string & path, std::string & name, std::vector<T> & data){

	const int dim = 1;
	const unsigned int size = data.size();

    hsize_t shape[dim] = {size};
    H5::DataType type = H5Type<T>();
    H5::DataType datatype(type);

	H5::H5File obj(path,H5F_ACC_TRUNC); // H5F_ACC_RDWR (TODO: Save multiple datasets to single file / overwrite datasets)

    H5::DataSpace dataspace(dim, shape);
    H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

    T * values = &data[0];

    dataset.write(values,datatype);

    return;
};


template<typename T>
void hdf5_to_eigen(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){
};

template<typename T>
void hdf5_to_vector(std::string & path, std::string & name, std::vector<T> & data){
};



template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> commutator<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &);
template Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> commutator<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &);

template void cast<double,double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<double,float>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<float,double>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,double>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & other);
template void cast<int,float>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & other);

template <> H5::DataType H5TypeMap<double>::h5_type = H5::PredType::NATIVE_DOUBLE;
template <> H5::DataType H5TypeMap<float>::h5_type = H5::PredType::NATIVE_FLOAT;
template <> H5::DataType H5TypeMap<int>::h5_type = H5::PredType::NATIVE_INT32;


template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data);
template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data);
template void eigen_to_hdf5(std::string & path, std::string & name, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data);


template void vector_to_hdf5(std::string & path, std::string & name, std::vector<double> & data);
template void vector_to_hdf5(std::string & path, std::string & name, std::vector<float> & data);
template void vector_to_hdf5(std::string & path, std::string & name, std::vector<int> & data);

};
