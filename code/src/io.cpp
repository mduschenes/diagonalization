#include "io.hpp"

namespace IO {

template <class T>
io<T>::io(char delimeter,char linebreak){
	this->delimeter=delimeter;
	this->linebreak=linebreak;
};

template <class T>
io<T>::io(char delimeter){
	this->delimeter=delimeter;
};

template <class T>
io<T>::io(){};

template <class T>
io<T>::~io(){};

template <class T>
void io<T>::write(std::string path,std::vector<std::string> & header,std::vector<std::vector<T>> & data){
	
	std::ofstream file(path);
	
	int N,M;
	_size(data,N,M);


	for(int i=0;i<M;i++){
		file << header[i];
		if (i<(M-1)){file<<this->delimeter;};
	};
	file << this->linebreak;

	for (int j=0;j<N;j++){
	for(int i=0;i<M;i++){
			if(j<data[i].size()){file << data[i][j];};
			if (i<(M-1)){file<<this->delimeter;};
		};
		file << this->linebreak;
	};

	file.close();
	return;	
};


template <class T>
void io<T>::read(std::string path,std::vector<std::string> & header,std::vector<std::vector<T>> & data){
	std::ifstream file(path);
	std::string line,string;
	T datum;

	int col=0,row=0;
	while(std::getline(file,line)){
		col=0;
		std::stringstream stream(line);

		if (row == 0){
			while(stream >> string){
				header.push_back(string);
				data.push_back(std::vector<T>{});
				if(stream.peek() == this->delimeter){stream.ignore();};
				col ++;
			};
		}
		else{
			while(stream >> datum){
				data[col].push_back(datum);
				col ++;
			};
		};
		row++;
	};
	file.close();
	return;
};

template <class T>
void  io<T>::_size(std::vector<std::vector<T>> & data, int &N, int &M){
	M = data.size();
	N = 0;
	int _N = 0;
	for(int i=0;i<M;i++){
		_N = data[i].size();
		if(_N>N){N=_N;};
	};
};


};
