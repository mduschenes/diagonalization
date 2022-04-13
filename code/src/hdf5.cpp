#include "hdf5.hpp"

namespace hdf5 {



// Eigen File Format
const static Eigen::IOFormat EigenFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");

template <typename T>
struct H5TypeMap {
  static H5::DataType h5_type;
};

template<typename T>
H5::DataType H5Type(){
  return H5TypeMap<T>::h5_type;
};


template<typename T>
void dump(std::string & path, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){

	H5::Exception::dontPrint();

	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	H5::H5File obj = file;

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

	T * values;

	data.transposeInPlace();

	values = data.data();

	dataset.write(values,datatype);

	data.transposeInPlace();

	return;
};

template<typename T>
void dump(std::string & path, std::string & name, Eigen::SparseMatrix<T> & data){

	H5::Exception::dontPrint();

	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	H5::H5File obj = file;

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

	// TODO: Memory Leak

	// T * values = data.coeffs().data();


	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> _data = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(data);
	T * values = _data.data();

	dataset.write(values,datatype);

	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){

	H5::Exception::dontPrint();

	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);
	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

	T * values = data.data();

	dataset.write(values,datatype);

	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<T> & data){

	H5::Exception::dontPrint();

	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);
	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

	// TODO: Memory Leak
	
	// T * values = data.coeffs().data();

	// dataset.write(values,datatype);

	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> _data = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(data);
	T * values = _data.data();

	dataset.write(values,datatype);


	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<T, Eigen::Dynamic> & data){

	H5::Exception::dontPrint();

	const int dim = 1;
	const unsigned int size = data.size();
	hsize_t shape[dim] = {size,};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = obj.createDataSet(name,datatype,dataspace);

	T * values = data.data();

	dataset.write(values,datatype);

	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & data){

	H5::Exception::dontPrint();
	
	unsigned int i,j;
	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};


	H5::Group obj = file.openGroup(group);


	std::string real = name;
	real.append(".real");

	std::string imag = name;
	imag.append(".imag");

	H5::DataSpace dataspace_real(dim, shape);
	H5::DataSet dataset_real = obj.createDataSet(real,datatype,dataspace_real);

	H5::DataSpace dataspace_imag(dim, shape);
	H5::DataSet dataset_imag = obj.createDataSet(imag,datatype,dataspace_imag);
	
	T values_real[row][col];
	T values_imag[row][col];

	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			values_real[i][j] = data.real()(i,j);    	
			values_imag[i][j] = data.imag()(i,j);
		};
	};

	dataset_real.write(values_real,datatype);
	dataset_imag.write(values_imag,datatype);

	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<T>> & data){

	H5::Exception::dontPrint();
	
	unsigned int i,j;
	const int dim = 2;
	const unsigned int row = data.rows();
	const unsigned int col = data.cols();
	hsize_t shape[dim] = {row,col};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};


	H5::Group obj = file.openGroup(group);


	std::string real = name;
	real.append(".real");

	std::string imag = name;
	imag.append(".imag");

	H5::DataSpace dataspace_real(dim, shape);
	H5::DataSet dataset_real = obj.createDataSet(real,datatype,dataspace_real);

	H5::DataSpace dataspace_imag(dim, shape);
	H5::DataSet dataset_imag = obj.createDataSet(imag,datatype,dataspace_imag);


	// TODO: Memory Leak
	
	T values_real[row][col];
	T values_imag[row][col];

	Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> _data = Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic>(data);

	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			values_real[i][j] = _data.real()(i,j);    	
			values_imag[i][j] = _data.imag()(i,j);
		};
	};

	dataset_real.write(values_real,datatype);
	dataset_imag.write(values_imag,datatype);

	return;
};

template<typename T>
void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data){

	H5::Exception::dontPrint();
	
	unsigned int i;
	const int dim = 1;
	const unsigned int size = data.size();
	hsize_t shape[dim] = {size,};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);


	std::string real = name;
	real.append(".real");

	std::string imag = name;
	imag.append(".imag");

	H5::DataSpace dataspace_real(dim, shape);
	H5::DataSet dataset_real = obj.createDataSet(real,datatype,dataspace_real);

	H5::DataSpace dataspace_imag(dim, shape);
	H5::DataSet dataset_imag = obj.createDataSet(imag,datatype,dataspace_imag);
	
	// T * values_real = data.real().data();
	// T * values_real = &std::reinterpret_cast<T(&)[2]>(data.real().data()[0] )[0]
	// Eigen::Map<Eigen::Vector<std::complex<T>, Eigen::Dynamic>(&values_real[0], size) = data.real();
	// T * values_imag = data.imag().data();
	// T * values_imag = &std::reinterpret_cast<T(&)[2]>( data.imag().data()[0] )[1]
	// Eigen::Map<Eigen::Vector<std::complex<T>, Eigen::Dynamic>(&values_imag[0], size) = data.imag();


	T values_real[size];
	T values_imag[size];

	for (i=0;i<size;i++){
		values_real[i] = data.real()(i);    	
		values_imag[i] = data.imag()(i);
	};

	dataset_real.write(values_real,datatype);
	dataset_imag.write(values_imag,datatype);

	return;
};



template<typename T>
void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,T> & attributes){

	H5::Exception::dontPrint();
	
	typename std::map<std::string, T>::iterator i;
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	std::filesystem::create_directory(std::filesystem::path(path).parent_path());

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);


	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);

	H5::DataSpace dataspace(H5S_SCALAR);

	H5::Attribute attribute;
	T * values;

	for (i=attributes.begin(); i!=attributes.end();i++){
		attribute = obj.createAttribute(i->first,datatype,dataspace);
		values = &attributes[i->first];
		attribute.write(datatype,values);
	};

	return;
};




template<typename T>
void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<T, Eigen::Dynamic>> & attributes){

	H5::Exception::dontPrint();
	
	const int dim = 1;
	unsigned int size;
	hsize_t shape[dim];
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	typename std::map<std::string, Eigen::Vector<T, Eigen::Dynamic>>::iterator i;

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);


	H5::DataSpace dataspace;
	H5::Attribute attribute;
	T * values;

	for (i=attributes.begin(); i!=attributes.end();i++){
		
		size = attributes[i->first].size();
		shape[0] = size;
		dataspace = H5::DataSpace(dim,shape);

		attribute = obj.createAttribute(i->first,datatype,dataspace);
		values = attributes[i->first].data();
		attribute.write(datatype,values);
	};

	return;
};


template<typename T>
void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> & attributes){

	H5::Exception::dontPrint();

	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	typename std::map<std::string, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>>::iterator i;

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	try {
		H5::Group obj = file.openGroup(group);
	}
	catch (...) {
		H5::Group obj = file.createGroup(group);
	};

	H5::Group obj = file.openGroup(group);


	H5::DataSpace dataspace;
	H5::Attribute attribute;
	T * values;

	for (i=attributes.begin(); i!=attributes.end();i++){
		
		if (attributes[i->first].cols() == 1){
			const int dim = 1;
			unsigned int size;
			hsize_t shape[dim];
			
			size = attributes[i->first].size();
			shape[0] = size;

			dataspace = H5::DataSpace(dim,shape);

			attribute = obj.createAttribute(i->first,datatype,dataspace);
			values = attributes[i->first].data();
			attribute.write(datatype,values);
		}
		else {
			const int dim = 2;
			const unsigned int row = attributes[i->first].rows();
			const unsigned int col = attributes[i->first].cols();
			hsize_t shape[dim] = {row,col};

			dataspace = H5::DataSpace(dim,shape);

			attribute = obj.createAttribute(i->first,datatype,dataspace);

			attributes[i->first].transposeInPlace();

			values = attributes[i->first].data();

			attribute.write(datatype,values);

			attributes[i->first].transposeInPlace();

		};

	};

	return;
};



template<typename T>
void dump(std::string & path, std::string & name, std::vector<T> & data){

	H5::Exception::dontPrint();

	const int dim = 1;
	const unsigned int size = data.size();

	hsize_t shape[dim] = {size};
	H5::DataType type = H5Type<T>();
	H5::DataType datatype(type);

	try {
		H5::H5File file(path,H5F_ACC_RDWR);
	}
	catch (...) {
		H5::H5File file(path,H5F_ACC_TRUNC);
	};
	H5::H5File file(path,H5F_ACC_RDWR);

	H5::DataSpace dataspace(dim, shape);
	H5::DataSet dataset = file.createDataSet(name,datatype,dataspace);

	T * values = &data[0];

	dataset.write(values,datatype);

	return;
};


template <> H5::DataType H5TypeMap<double>::h5_type = H5::PredType::NATIVE_DOUBLE;
template <> H5::DataType H5TypeMap<float>::h5_type = H5::PredType::NATIVE_FLOAT;
template <> H5::DataType H5TypeMap<int>::h5_type = H5::PredType::NATIVE_INT32;
template <> H5::DataType H5TypeMap<unsigned int>::h5_type = H5::PredType::NATIVE_INT32;


template void dump(std::string & path, std::string & name, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & name, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & name, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & name, Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic> & data);

template void dump(std::string & path, std::string & name, Eigen::SparseMatrix<double> & data);
template void dump(std::string & path, std::string & name, Eigen::SparseMatrix<float> & data);
template void dump(std::string & path, std::string & name, Eigen::SparseMatrix<int> & data);
template void dump(std::string & path, std::string & name, Eigen::SparseMatrix<unsigned int> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<double> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<float> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<int> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<unsigned int> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<double, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<float, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<int, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<unsigned int, Eigen::Dynamic> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<int>, Eigen::Dynamic, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Matrix<std::complex<unsigned int>, Eigen::Dynamic, Eigen::Dynamic> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<double>> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<float>> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<int>> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::SparseMatrix<std::complex<unsigned int>> & data);

template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<double>, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<float>, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<int>, Eigen::Dynamic> & data);
template void dump(std::string & path, std::string & group, std::string & name, Eigen::Vector<std::complex<unsigned int>, Eigen::Dynamic> & data);

template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,double> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,float> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,int> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,unsigned int> & attributes);


template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<double, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<float, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<int, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Vector<unsigned int, Eigen::Dynamic>> & attributes);

template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>> & attributes);
template void dump(std::string & path, std::string & group, std::string & name, std::map<std::string,Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic>> & attributes);

template void dump(std::string & path, std::string & name, std::vector<double> & data);
template void dump(std::string & path, std::string & name, std::vector<float> & data);
template void dump(std::string & path, std::string & name, std::vector<int> & data);
template void dump(std::string & path, std::string & name, std::vector<unsigned int> & data);

};