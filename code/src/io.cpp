#include "io.hpp"

namespace io {

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
	
	unsigned int N,M;
	this->shape(data,N,M);

	for(unsigned int i=0;i<M;i++){
		file << header[i];
		if (i<(M-1)){file<<this->delimeter;};
	};
	file << this->linebreak;

	for (unsigned int j=0;j<N;j++){
	for(unsigned int i=0;i<M;i++){
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

	int number = 0;
	while(std::getline(file,line)){
		
		std::istringstream stream(line);
		data.push_back(std::vector<T>{});

		if (number == 0){
			while(std::getline(stream, string, this->delimeter)){
				header.push_back(string);
			};
		}
		else{
			while(std::getline(stream, string, this->delimeter)){
				datum = this->parse(string);
				data[number].push_back(datum);
			};
		};
		number++;
	};
	file.close();
	return;
};



template <class T>
void io<T>::write(std::string path,std::vector<std::vector<T>> & data){
	
	std::ofstream file(path);
	
	unsigned int N,M;
	this->shape(data,N,M);

	for (unsigned int j=0;j<N;j++){
		for(unsigned int i=0;i<M;i++){
			if(j<data[i].size()){file << data[i][j];};
			if (i<(M-1)){file<<this->delimeter;};
		};
		file << this->linebreak;
	};

	file.close();
	return;	
};


template <class T>
void io<T>::read(std::string path,std::vector<std::vector<T>> & data){

	std::ifstream file(path);	
	std::string line,string;	
	T datum;

	int number = 0;
	while(std::getline(file,line)){

		std::istringstream stream(line);
		data.push_back(std::vector<T>{});
		
		while(std::getline(stream, string, this->delimeter)){
			datum = this->parse(string);
			data[number].push_back(datum);
		};

		number++;
	};
	file.close();
	return;
};


template <class T>
void io<T>::shape(std::vector<std::vector<T>> & data, unsigned int &N, unsigned int &M){
	M = data.size();
	N = 0;
	unsigned int _N = 0;
	for(unsigned int i=0;i<M;i++){
		_N = data[i].size();
		if(_N>N){N=_N;};
	};
};


template<class T>
T io<T>::parse(std::string & string){
	std::vector<char> remove = {' ',};
	for (char s : remove){
		string.erase(std::remove(string.begin(), string.end(),s), string.end());
	};
	// string.erase(std::remove(string.begin(), string.end(),' '), string.end());
	std::istringstream stream(string);
    T value;
    stream >> value;	
	return value;
}

template class io<double>;
template class io<int>;


};


