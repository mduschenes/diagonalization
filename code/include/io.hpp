#ifndef _IO_
#define _IO_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


namespace IO {

template <class T>
class io{
	public:
		io(char delimeter, char linebreak);
		io(char delimeter);
		io();
		~io();

		char delimeter = {','};
		char linebreak = {'\n'};

		void write(std::string path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);
		void read(std::string path,std::vector<std::string> & header,std::vector<std::vector<T>> & data);

	private:
		void _size(std::vector<std::vector<T>> & data,int & N,int & M);
};		

template class io<int>;
template class io<float>;
template class io<double>;
template class io<char>;
template class io<std::string>;
};
#endif