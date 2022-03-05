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
void io<T>::write(std::string & path,std::vector<std::string> & header,std::vector<T> & data){
	
	std::ofstream file(path);
	
	unsigned int N;
	this->shape(data,N);

	for(unsigned int i=0;i<N;i++){
		file << header[i];
		if (i<(N-1)){file<<this->delimeter;};
	};
	file << this->linebreak;

	for (unsigned int i=0;i<N;i++){
		if(i<data.size()){file << data[i];};
		if (i<(N-1)){file<<this->delimeter;};
		};

	file.close();
	return;	
};



template <class T>
void io<T>::read(std::string & path,std::vector<std::string> & header,std::vector<T> & data){
	std::ifstream file(path);
	std::string line,string;
	T value;

	int number = 0;
	while(std::getline(file,line)){
		
		std::istringstream stream(line);

		if (number == 0){
			while(std::getline(stream, string, this->delimeter)){
				header.push_back(string);
			};
		}
		else if (number == 1){
			while(std::getline(stream, string, this->delimeter)){
				this->parse(value,string);
				data.push_back(value);
			};
		}
		else{			
		};
		number++;
	};
	file.close();
	return;
};



template <class T>
void io<T>::write(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data){
	
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
void io<T>::read(std::string & path,std::vector<std::string> & header,std::vector<std::vector<T>> & data){
	std::ifstream file(path);
	std::string line,string;
	T value;

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
				this->parse(value,string);
				data[number].push_back(value);
			};
		};
		number++;
	};
	file.close();
	return;
};



template <class T>
void io<T>::write(std::string & path,std::vector<std::vector<T>> & data){
	
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
void io<T>::read(std::string & path,std::vector<T> & data){

	std::ifstream file(path);	
	std::string string;	
	T value;

	while(std::getline(file,string, this->delimeter)){
		this->parse(value,string);
		data.push_back(value);
	};
	file.close();
	return;
};



template <class T>
void io<T>::write(std::string & path,std::vector<T> & data){
	
	std::ofstream file(path);

	unsigned int N;
	this->shape(data,N);

	for (unsigned int i=0;i<N;i++){
		if(i<data.size()){file << data[i];};
	};
	file.close();
	return;	
};


template <class T>
void io<T>::read(std::string & path,std::vector<std::vector<T>> & data){

	std::ifstream file(path);	
	std::string line,string;	
	T value;

	int number = 0;
	while(std::getline(file,line)){

		std::istringstream stream(line);
		data.push_back(std::vector<T>{});
		
		while(std::getline(stream, string, this->delimeter)){
			this->parse(value,string);
			data[number].push_back(value);
		};

		number++;
	};
	file.close();
	return;
};


template <class T>
void io<T>::write(std::string & path,Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & data){
	std::string name = "data";
	utils::eigen_to_hdf5<T>(path,name,data);
	return;
};


template <class T>
void io<T>::join(std::string & path,std::string & directory,std::string & file,std::string & ext){
	if (path.size()>0){
		path.append("/");	
	};
    path.append(directory);
	path.append("/");	    
    path.append(file);
	path.append(".");
	path.append(ext);
	return;

};


template<class T>
void io<T>::parse(T & value, std::string & string){
	std::vector<char> remove = {' ',};
	for (char s : remove){
		string.erase(std::remove(string.begin(), string.end(),s), string.end());
	};
	std::istringstream stream(string);
    stream >> value;	
    return;
};


template <class T>
void io<T>::shape(std::vector<T> & data, unsigned int &N){
	N = data.size();
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
	return;	
};


template class io<double>;
template class io<int>;


};


