#ifndef _IO_
#define _IO_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

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

		void write(std::string & path,std::vector<std::string> & header,std::vector<T> & data);
		void read(std::string & path,std::vector<std::string> & header,std::vector<T> & data);

		void write(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);
		void read(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);

		void write(std::string & path,std::vector<T> & data);
		void read(std::string & path,std::vector<T> & data);

		void write(std::string & path,std::vector<std::vector<T>> & data);
		void read(std::string & path,std::vector<std::vector<T>> & data);

		void write(std::string & path,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data);

		void join(std::string & path,std::string & directory,std::string & file,std::string & ext);
		void parse(T & value, std::string & string);

	private:
		void shape(std::vector<T> & data,unsigned int & N);
		void shape(std::vector<std::vector<T>> & data,unsigned int & N,unsigned int & M);
};		

};

#endif