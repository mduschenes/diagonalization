#include "linalg.hpp"

namespace linalg {


template<typename T>
void eigh(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

	typename Eigen::SelfAdjointEigenSolver<type> solver;

	solver.compute(matrix);
	
	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors();

	return;
};

template<typename T>
void eigh(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::SparseMatrix<T,Eigen::ColMajor> type;
	typedef typename Eigen::SimplicialLDLT<type> algorithm;	
	int options = Eigen::ComputeEigenvectors;

	Eigen::ArpackGeneralizedSelfAdjointEigenSolver<type,algorithm> solver;

	solver.compute(matrix,n,sigma,options,eps); // https://docs.scipy.org/doc/scipy/tutorial/arpack.html

	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors();

	return;
};

template<typename T>
void eigh(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type;

	typename Eigen::SelfAdjointEigenSolver<type> solver;

	solver.compute(matrix);
	
	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors();

	return;
};




template<typename T>
void eigh(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){

	typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

	typename Eigen::SelfAdjointEigenSolver<type> solver;

	solver.compute(matrix);
	
	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors().real();

	return;
};

template<typename T>
void eigh(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::SparseMatrix<T,Eigen::ColMajor> type;
	typedef typename Eigen::SimplicialLDLT<type> algorithm;	
	int options = Eigen::ComputeEigenvectors;

	Eigen::ArpackGeneralizedSelfAdjointEigenSolver<type,algorithm> solver;

	solver.compute(matrix,n,sigma,options,eps); // https://docs.scipy.org/doc/scipy/tutorial/arpack.html

	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors().real();

	return;
};

template<typename T>
void eigh(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type;

	typename Eigen::SelfAdjointEigenSolver<type> solver;

	solver.compute(matrix);
	
	eigenvalues = solver.eigenvalues();
	eigenvectors = solver.eigenvectors().real();

	return;
};

// template<typename T>
// void eigh(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> type;
// 	typedef typename Eigen::SimplicialLDLT<type> algorithm;	
// 	int options = Eigen::ComputeEigenvectors;

// 	typename Eigen::ArpackGeneralizedSelfAdjointEigenSolver<type,algorithm> solver;

// 	solver.compute(matrix,n,sigma,options,eps); // https://docs.scipy.org/doc/scipy/tutorial/arpack.html

// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors().real();

// 	return;
// };




// template<typename T>
// void eigs(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;
// 	Spectra::SortRule sort = Spectra::SortRule::LargestMagn;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};

// 	typename Spectra::DenseSymMatProd<T> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::DenseSymMatProd<T>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };


// template<typename T>
// void eigs(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<T,Eigen::ColMajor> type;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};
				
// 	typename Spectra::SparseSymMatProd<T> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::SparseSymMatProd<T>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };

// template<typename T>
// void eigs(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type;
// 	Spectra::SortRule sort = Spectra::SortRule::LargestMagn;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};

// 	typename Spectra::DenseSymMatProd<std::complex<T>> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::DenseSymMatProd<std::complex<T>>> solver(op,n,eps);
// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };


// template<typename T>
// void eigs(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> type;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};
				
// 	typename Spectra::SparseSymMatProd<std::complex<T>> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::SparseSymMatProd<std::complex<T>>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };



// template<typename T>
// void eigh(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> type;
// 	typedef typename Eigen::SimplicialLDLT<type> algorithm;	
// 	int options = Eigen::ComputeEigenvectors;

// 	typename Eigen::ArpackGeneralizedSelfAdjointEigenSolver<type,algorithm> solver;

// 	solver.compute(matrix,n,sigma,options,eps); // https://docs.scipy.org/doc/scipy/tutorial/arpack.html

// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };

// template<typename T>
// void eigs(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;
// 	Spectra::SortRule sort = Spectra::SortRule::LargestMagn;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};

// 	typename Spectra::DenseSymMatProd<T> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::DenseSymMatProd<T>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };


// template<typename T>
// void eigs(Eigen::SparseMatrix<T,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<T,Eigen::ColMajor> type;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};
				
// 	typename Spectra::SparseSymMatProd<T> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::SparseSymMatProd<T>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };

// template<typename T>
// void eigs(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type;
// 	Spectra::SortRule sort = Spectra::SortRule::LargestMagn;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};

// 	typename Spectra::DenseSymMatProd<std::complex<T>> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::DenseSymMatProd<std::complex<T>>> solver(op,n,eps);
// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };


// template<typename T>
// void eigs(Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> & matrix, Eigen::Vector<T, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, T & eps){
// 	typedef typename Eigen::SparseMatrix<std::complex<T>,Eigen::ColMajor> type;
// 	int options = Eigen::ComputeEigenvectors;
// 	Spectra::SortRule sort;
// 	if (sigma == "LM"){
// 		sort = Spectra::SortRule::LargestMagn;
// 	}
// 	else if (sigma == "LA"){
// 		sort = Spectra::SortRule::LargestAlge;
// 	}	
// 	else if (sigma == "LR"){
// 		sort = Spectra::SortRule::LargestReal;
// 	}
// 	else if (sigma == "LI"){
// 		sort = Spectra::SortRule::LargestImag;
// 	}
// 	else if (sigma == "SM"){
// 		sort = Spectra::SortRule::SmallestMagn;
// 	}
// 	else if (sigma == "SA"){
// 		sort = Spectra::SortRule::SmallestAlge;
// 	}	
// 	else if (sigma == "SR"){
// 		sort = Spectra::SortRule::SmallestReal;
// 	}
// 	else if (sigma == "SI"){
// 		sort = Spectra::SortRule::SmallestImag;
// 	}	
// 	else if (sigma == "BE"){
// 		sort = Spectra::SortRule::BothEnds;
// 	}
// 	else {
// 		sort = Spectra::SortRule::LargestMagn;
// 	};
				
// 	typename Spectra::SparseSymMatProd<std::complex<T>> op(matrix);
// 	typename Spectra::SymEigsSolver<Spectra::SparseSymMatProd<std::complex<T>>> solver(op,n,eps);

// 	solver.init();
// 	solver.compute(sort);
	
// 	eigenvalues = solver.eigenvalues();
// 	eigenvectors = solver.eigenvectors();

// 	return;
// };


template<typename T>
void svd(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

	Eigen::JacobiSVD<type> solver;

	solver.compute(matrix);
	singularvalues = solver.singularValues();

	return;
};

template<typename T>
void svd(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<T, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, T & eps){
	typedef typename Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> type;

	Eigen::JacobiSVD<type> solver;

	solver.compute(matrix);
	singularvalues = solver.singularValues();

	return;
};


template<typename T>
T norm(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & b){
	return (a.transpose()*b).trace();
};


template<typename T>
T norm(Eigen::SparseMatrix<T> & a, Eigen::SparseMatrix<T> & b){
	return 0;
};

template <typename T>
void permute(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = utils::Vector(indices);
	if (axis == 0){
		a = permuation * a;		
	}
	else if ((axis == 1) or (axis == -1)){
		a = a * permuation;		
	}
	else {
		a = permuation * a;		
	};
	return;
};


template <typename T>
void permute(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = utils::Vector(indices);
	if (axis == 0){
		a = permuation * a;		
	}
	else if ((axis == 1) or (axis == -1)){
		a = a * permuation;		
	}
	else {
		a = permuation * a;		
	};
	return;
};

template <typename T>
void permute(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = utils::Vector(indices);
	a = permuation * a;	
	return;
};

template <typename T>
void permute(Eigen::Vector<std::complex<T>, Eigen::Dynamic> & a, std::vector<int> & indices){
	Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permuation;
	permuation.indices() = utils::Vector(indices);
	a = permuation * a;	
	return;
};


template <typename T>
void argsort(Eigen::Vector<T, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting){

	indices.resize(a.size());
	std::iota(indices.begin(),indices.end(),0);

	bool (*algorithm)(T,T);
	if (sorting == "<="){
		algorithm = [](T a, T b) {return a <= b;};
	}
	else if (sorting == "<"){
		algorithm = [](T a, T b) {return a < b;};
	}
	else if (sorting == ">="){
		algorithm = [](T a, T b) {return a >= b;};
	}
	else if (sorting == ">"){
		algorithm = [](T a, T b) {return a > b;};
	}
	else {
		algorithm = [](T a, T b) {return a <= b;};
	};

	std::stable_sort(indices.begin(), indices.end(),[&a,&algorithm](unsigned int i, unsigned int j) {return algorithm(a(i),a(j));}); 

	return;
};

template<typename T>
void trace(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, T & eps){
	Eigen::array<int,2> shape({(int)n,(int)n});
	unsigned int row = matrix.rows();
	unsigned int col = matrix.cols();
	Eigen::array<int,2> dims({1,3});

	Eigen::TensorMap<Eigen::Tensor<T,4>> tensor(matrix.data(),n,row/n,n,col/n);
	Eigen::Tensor<T,2> traced = tensor.trace(dims);

	matrix = Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>>(traced.data(),shape[0],shape[1]);
	
	return;
};

template<typename T>
void trace(Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, T & eps){
	Eigen::array<int,2> shape({(int)n,(int)n});
	unsigned int row = matrix.rows();
	unsigned int col = matrix.cols();
	Eigen::array<int,2> dims({1,3});

	Eigen::TensorMap<Eigen::Tensor<std::complex<T>,4>> tensor(matrix.data(),n,row/n,n,col/n);
	Eigen::Tensor<std::complex<T>,2> traced = tensor.trace(dims);
	
	matrix = Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic>>(traced.data(),shape[0],shape[1]);
	
	return;
};


template void eigh<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

template void eigh<double>(Eigen::SparseMatrix<double,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::SparseMatrix<float,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

template void eigh<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

// template void eigh<double>(Eigen::SparseMatrix<std::complex<double>,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigh<float>(Eigen::SparseMatrix<std::complex<float>,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

template void eigh<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

template void eigh<double>(Eigen::SparseMatrix<double,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::SparseMatrix<float,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

template void eigh<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
template void eigh<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

// template void eigh<double>(Eigen::SparseMatrix<std::complex<double>,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigh<float>(Eigen::SparseMatrix<std::complex<float>,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);


// template void eigs<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigs<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

// template void eigs<double>(Eigen::SparseMatrix<double,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigs<float>(Eigen::SparseMatrix<float,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

// template void eigs<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigs<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);

// template void eigs<double>(Eigen::SparseMatrix<std::complex<double>,Eigen::ColMajor> & matrix, Eigen::Vector<double, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, double & eps);
// template void eigs<float>(Eigen::SparseMatrix<std::complex<float>,Eigen::ColMajor> & matrix, Eigen::Vector<float, Eigen::Dynamic> & eigenvalues, Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & eigenvectors, unsigned int & n, std::string & sigma, float & eps);


template void svd<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, double & eps);
template void svd<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, float & eps);

template void svd<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<double, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, double & eps);
template void svd<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & matrix, Eigen::Vector<float, Eigen::Dynamic> & singularvalues, unsigned int & n, std::string & sigma, float & eps);

template double norm<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & b);
template float norm<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & b);
template int norm<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & a,Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & b);

template double norm<double>(Eigen::SparseMatrix<double> & a,Eigen::SparseMatrix<double> & b);
template float norm<float>(Eigen::SparseMatrix<float> & a,Eigen::SparseMatrix<float> & b);
template int norm<int>(Eigen::SparseMatrix<int> & a,Eigen::SparseMatrix<int> & b);

template void permute<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<int>(Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<unsigned int>(Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);

template void permute<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<int>(Eigen::Matrix<std::complex<int>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);
template void permute<unsigned int>(Eigen::Matrix<std::complex<unsigned int>, Eigen::Dynamic, Eigen::Dynamic> & a, std::vector<int> & indices,int axis);

template void permute<double>(Eigen::Vector<double, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<float>(Eigen::Vector<float, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<int>(Eigen::Vector<int, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<unsigned int>(Eigen::Vector<unsigned int, Eigen::Dynamic> & a, std::vector<int> & indices);

template void permute<double>(Eigen::Vector<std::complex<double>, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<float>(Eigen::Vector<std::complex<float>, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<int>(Eigen::Vector<std::complex<int>, Eigen::Dynamic> & a, std::vector<int> & indices);
template void permute<unsigned int>(Eigen::Vector<std::complex<unsigned int>, Eigen::Dynamic> & a, std::vector<int> & indices);

template void argsort<double>(Eigen::Vector<double, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);
template void argsort<float>(Eigen::Vector<float, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);
template void argsort<int>(Eigen::Vector<int, Eigen::Dynamic> & a, std::vector<int> & indices, std::string & sorting);

template void trace<double>(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, double & eps);
template void trace<float>(Eigen::Matrix<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, float & eps);

template void trace<double>(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, double & eps);
template void trace<float>(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> & matrix, unsigned int & n, float & eps);

};
