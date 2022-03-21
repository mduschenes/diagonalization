#ifndef _IO_
#define _IO_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#define EIGEN_USE_MKL_ALL   
#define NUM_THREADS 7
#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 

#include "hdf5.hpp"
#include "utils.hpp"

namespace io {

template <class T>
class io{
	public:
		io(char delimeter, char linebreak);
		io(char delimeter);
		io();
		~io();

		char delimeter = {','};
		char linebreak = {'\n'};

		void dump(std::string & path,std::vector<std::string> & header,std::vector<T> & data);
		void load(std::string & path,std::vector<std::string> & header,std::vector<T> & data);

		void dump(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);
		void load(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);

		void dump(std::string & path,std::vector<T> & data);
		void load(std::string & path,std::vector<T> & data);

		void dump(std::string & path,std::string & name, std::vector<T> & data);
		void dump(std::string & path,std::string & name, std::vector<std::string> & header, std::vector<T> & data);

		void dump(std::string & path,std::vector<std::vector<T>> & data);
		void load(std::string & path,std::vector<std::vector<T>> & data);

		void dump(std::string & path,std::string & group,std::string & name,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);
		void dump(std::string & path,std::string & group,std::string & name,Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & data);
		void dump(std::string & path,std::string & group,std::string & name,Eigen::Vector<T, Eigen::Dynamic> & data);
		void dump(std::string & path,std::string & group,std::string & name,Eigen::Vector<std::complex<T>, Eigen::Dynamic> & data);

		void dump(std::string & path,std::string & group,std::string & name, std::map<std::string,T> & attributes);
		void dump(std::string & path,std::string & group,std::string & name, std::map<std::string,Eigen::Vector<T, Eigen::Dynamic>> & attributes);

		void join(std::string & path,std::string & directory,std::string & file,std::string & ext);
		void parse(T & value, std::string & string);

	private:
		void shape(std::vector<T> & data,unsigned int & N);
		void shape(std::vector<std::vector<T>> & data,unsigned int & N,unsigned int & M);
};		

};

#endif