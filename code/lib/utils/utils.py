#!/usr/bin/env python

# Import python modules
import os,sys,itertools,functools,math,copy

import matplotlib
import matplotlib.pyplot as plt

import numpy as np
import scipy as sp
import pandas as pd
import sparse as sparray
import jax
import jax.numpy as jnp
import jax.scipy as jsp
jax.config.update('jax_platform_name','cpu')
jax.config.update("jax_enable_x64", True)
# jax.set_cpu_device_count(8)
# os.env['XLA_FLAGS'] ='--xla_force_host_platform_device_count=8'

# Logging
import logging
logger = logging.getLogger(__name__)



def jit(func,*,static_argnums=None):
	# return func
	return jax.jit(func,static_argnums=static_argnums)


class dictionary(dict):
	'''
	Dictionary subclass with dictionary elements explicitly accessible as class attributes
	Args:
		args (dict): Dictionary elements
		kwargs (dict): Dictionary elements
	'''
	def __init__(self,*args,**kwargs):
		
		args = {k:v for a in args for k,v in ({} if a is None else a).items()}		
		kwargs = kwargs
		attrs = {**args,**kwargs}

		for attr in attrs:
			setattr(self,attr,attrs[attr])

		super().__init__(attrs)

		return

	def __getattribute__(self,item):
		return super().__getattribute__(item)

	# def __getattr__(self,item):
	# 	return super().__getattr__(item)

	# def __setattr__(self,item,value):
	# 	return super().__setitem__(item,value)

	def __getitem__(self,item):
		return super().__getitem__(item)

	def __setitem__(self,item,value):
		return super().__setitem__(item,value)

	def __iter__(self):
		return super().__iter__()

	def __len__(self):
		return super().__len__()


class Array(np.ndarray):
	'''
	Numpy array subclass, subclass of nd.ndarray with array of data of shape
	
	Classes that inherit Array must have a __setup__() in place of __init__() to properly initialize
	subclassed ndarray, that is initalized through Array's __new__() and an array.view(cls)
	
	Classes that inherit Array's __setup__() create the following attributes:
		self.data (array)
		self.string (str)

	Classes that inherit Array's __setup__() must also append newly institated attributes to the 
	self.attrs attribute dictionary to allow for proper re-instantiation of the inherited class 
	upon numpy-like views, slices etc.

	Args:
		data (array,list,tuple): array data if array or list, array shape if tuple
		args (iterable): class attributes
		kwargs (dict): class keyword attributes    
	'''
	def __new__(cls,*args,**kwargs):
		attrs = {}
		clsattrs = {}
		for attr,value in kwargs.items():
			if attr in np.ndarray.__dict__:
				clsattrs[attr] = value
			else:
				attrs[attr] = value

		field,value = 'attrs',attrs
		setattr(cls,field,value)

		cls = cls.__new__(cls,*args,**kwargs)

		obj = np.asarray(cls.data,**clsattrs).view(cls)

		field,value = 'attrs',attrs
		setattr(obj,field,value)

		for attr,value in attrs.items():
			setattr(obj,attr,getattr(obj,attr,value))

		return obj

	def __array_finalize__(self, obj):
		field = 'attrs'
		attrs = getattr(obj,field,getattr(self,field,None))
		if obj is None or attrs is None: 
			return
		for attr in attrs:
			setattr(self,attr,getattr(obj,attr,getattr(self,attr,None)))
		return

	
	def __setup__(self,*args,**kwargs):
		
		field = 'attrs'
		getattr(self,field).update({attr:getattr(self,attr,None) for attr in kwargs if attr not in getattr(self,field)})
		
		data = args[0] if len(args)>0 else None
		if data is None:
			data = np.array()
		elif isinstance(data,tuple):
			data = np.zeros(data)
		else:
			data = np.asarray(data)
		self.data = data
		self.string = data.__str__()
		return

	def __repr__(self):
		return self.string

	def to_numpy(self):
		return np.asarray(self)

	def to_jax(self):
		return jnp.asarray(self)

	def log(self,msg):
		'''
		Log messages
		Args:
			msg (str): Message to log
		'''
		logger.log(self.verbose,msg)
		return

class String(str):
	'''
	String class to represent concatenated and delimited strings, subclass of str
	Args:
		string (str,tuple,list): string, tuple of strings, or list of tuple of strings or strings, where strings in tuple are concatenated and strings in list are delimited		
	'''
	def __new__(cls,string,deliminate=' + ',concatenate='*',fmt=None):
		try:
			string = [tuple([substring]) if not isinstance(substring,tuple) else substring 
						for substring in ([string] if not isinstance(string,list) else string)]
			string = deliminate.join([concatenate.join(substring) for substring in string])
		except:
			string = ''
		string = fmt%string if fmt is not None else string
		obj = super().__new__(cls,string)
		obj.deliminate = deliminate
		obj.concatenate = concatenate			
		return obj

	def __getslice__(self,item):
		try:
			obj = self.deliminate.join(self.split(self.deliminate)[item])
		except:
			obj = self
		return obj

	def __getitem__(self,item):
		try:
			obj = self.split(self.deliminate)[item]
		except:
			obj = self
		return obj


class System(dictionary):
	'''
	System attributes (dtype,format,device,seed,verbose,...)
	Args:
		dtype (str,data-type): Data type of class
		format (str): Format of array
		device (str): Device for computation
		seed (array,int): Seed for random number generation
		verbose (bool,str): Verbosity of class	
		args (dict,System): Additional system attributes
		kwargs (dict): Additional system attributes
	'''
	def __init__(self,*args,**kwargs):


		updates = {
			'verbose':{
				'notset':0,'debug':10,'info':20,'warning':30,'error':40,'critical':50,
				'Notset':0,'Debug':10,'Info':20,'Warning':30,'Error':40,'Critical':50,
				'NOTSET':0,'DEBUG':10,'INFO':20,'WARNING':30,'ERROR':40,'CRITICAL':50,
				10:10,20:20,30:30,40:40,50:50,
				2:20,3:30,4:40,5:50,
				True:20,False:0,None:0,
				}
			}

		defaults = {
			'dtype':'complex',
			'format':'array',
			'device':'cpu',
			'seed':None,
			'verbose':False,
		}

		args = {k:v for a in args for k,v in ({} if a is None else a).items()}
		attrs = {**args,**kwargs}
		attrs.update({attr: defaults[attr] for attr in defaults if attrs.get(attr) is None})
		attrs.update({attr: updates.get(attr,{}).get(attrs[attr],attrs[attr]) for attr in attrs})

		super().__init__(**attrs)

		return


class objs(np.ndarray):
	'''
	array class of objects
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return np.array(*args,**kwargs)


class asobjs(np.ndarray):
	'''
	array class of objects
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return np.asarray(*args,**kwargs)


class array(jnp.ndarray):
	'''
	array class
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.array(*args,**kwargs))


class asarray(np.ndarray):
	'''
	array class
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.asarray(*args,**kwargs))

class ones(array):
	'''
	array class of ones
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.ones(*args,**kwargs))


class zeros(array):
	'''
	array class of zeros
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.zeros(*args,**kwargs))

class eye(array):
	'''
	array class of eye
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.eye(*args,**kwargs))

class arange(array):
	'''
	array class of arange
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,*args,**kwargs):
		return jax.device_put(jnp.arange(*args,**kwargs))


class identity(array):
	'''
	array class of identity
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,n,*args,**kwargs):
		return jax.device_put(jnp.eye(n,*args,**kwargs))


class hadamard(array):
	'''
	array class of hadamard
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,n,*args,**kwargs):
		if n == 1:
			out = array(1,*args,**kwargs)
			return out
		else:
			out = hadamard(n-1,*args,**kwargs)
			return 1/sqrt(2)*array([[out,out],[out,-out]],*args,**kwargs)


class phasehadamard(array):
	'''
	array class of phaseshadamard
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,n,*args,**kwargs):
		assert n == 2, "phasehadamard only defined for n = 2"
		if n == 1:
			out = array(1,*args,**kwargs)
			return out
		elif n == 2:
			out = 1/sqrt(2)*array([[1,1],[1j,-1j]],*args,**kwargs)
			return out
		else:
			out = phasehadamard(n-1,*args,**kwargs)
			return 1/sqrt(2)*array([[out,out],[out,-out]],*args,**kwargs)


class cnot(array):
	'''
	array class of cnot
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,n,*args,**kwargs):
		assert n == 4, "cnot only defined for n = 4"
		if n == 1:
			out = array(1,*args,**kwargs)
			return out
		elif n == 4:
			out = array([])
			out = array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]],*args,**kwargs)
			return out
		else:
			out = cnot(n-1,*args,**kwargs)
			return array([[out,out],[out,-out]],*args,**kwargs)


class toffoli(array):
	'''
	array class of toffoli
	Args:
		args (iterable): Array arguments
		kwargs (dict): Array keyword arguments
	Returns:
		out (array): array
	'''
	def __new__(self,n,*args,**kwargs):
		assert n == 8, "toffoli only defined for n = 8"
		if n == 1:
			out = array(1,*args,**kwargs)
			return out
		elif n == 8:
			out = array([])
			out = array([
				[1,0,0,0,0,0,0,0],
				[0,1,0,0,0,0,0,0],
				[0,0,1,0,0,0,0,0],
				[0,0,0,1,0,0,0,0],
				[0,0,0,0,1,0,0,0],
				[0,0,0,0,0,1,0,0],
				[0,0,0,0,0,0,0,1],
				[0,0,0,0,0,0,1,0]]
				,*args,**kwargs)
			return out
		else:
			out = toffoli(n-1,*args,**kwargs)
			return array([[out,out],[out,-out]],*args,**kwargs)

def svd(A,k=None):
	'''
	Perform SVD on array, possibly reduced rank k
	Args:
		A (array): array of shape (n,m)
		k (int): reduced rank of SVD, defaults to max(n,m) if None
	Returns
		U (array): unitary of left eigenvectors of A of shape (n,k)
		S (array): array of singular values of shape (k,)
		V (array): conjugate of unitary of right eigenvectors of A of shape (k,m)
	'''

	n,m = A.shape

	U,S,V = np.linalg.svd(A)

	p,r,q = U.shape[0],S.size,V.shape[0]

	if k > p:
		U = np.concatenate((U,np.zeros((n,k-p))),axis=1)
	else:
		U = U[:,:k]
	if k > r:
		S = np.concatenate((S,np.zeros(k-r)),axis=0)
	else:
		S = S[:k]		
	if k > q:
		V = np.concatenate((V,np.zeros((k-q,m))),axis=0)
	else:
		V = V[:k,:]


	S = np.diag(S)

	return U,S,V


@functools.partial(jit,static_argnums=(1,2,))
def norm(arr,axis=None,ord=2):
	'''
	Norm of array
	Args:
		arr (array): array to be normalized
		axis (int): axis to normalize over. Flattens array if None.
		ord (int,str): order of normalization
	'''

	if axis is None:
		arr = arr.ravel()

	out = jnp.linalg.norm(arr,axis=axis,ord=ord)

	return out



def powerset(iterable,probability=1):
	'''
	Get power set of iterable
	Args:
		iterable (iterable): iterable to get powerset of
		probability (float): probability of returning element from power set
	Yields:
		iteration (iterable): iteration of powerset of iterable
	'''

	for number,iteration in enumerate(itertools.product(*[[[], [i]] for i in iterable])):
		iteration = (j for i in iteration for j in i)
		if (probability==1) or (np.random.rand() >= probability):
			yield iteration
		else:
			continue


def multiply(a):
	'''
	Multiply list of arrays elementwise
	Args:
		a: Arrays to multiply
	Returns:
		out (ndarray) if out argument
	'''
	out = a[0]

	sparse = issparse(out)

	for b in a[1:]:
		if not sparse:
			out = out*b
		else:
			out = out.multiply(b)
	
	return out

def add(a):
	'''
	Add list of arrays elementwise
	Args:
		a (iterable): Arrays to add
	Returns:
		out (ndarray) if out argument
	'''
	out = arrs[0]

	for arr in arrs[1:]:
		out = out + arr

	return out


@jit
def swap(i,j,N,D):
	'''
	Swap array elements acting on partition i <-> j on N partition array, each of D dimensions
	Args:
		i (int): partition to swap
		j (int): partition to swap
		N (int): Number of partitions
		D (int): Dimension of each partition
	Returns:
		S (array): Swap array
	'''
	s = eye(D,dtype=int)
	I = [s.copy() for n in range(N)]
	S = 0
	# jax.lax.
	for a in range(D):
		for b in range(D):
			I[i][:],I[j][:] = 0,0
			I[i][a][b],I[j][b][a] = 1,1
			S += tensorprod(I)
			I[i][:],I[j][:] = s,s
	return S



@functools.partial(jit,static_argnums=(1,2,))	
def slices(a,start,size):
	'''
	Get slice of array
	Args:
		a (array): Array to be sliced along first dimension
		start (int): Start index to slice
		size (int): Length of slice
	Returns:
		a (array): Sliced array
	'''
	return jax.lax.dynamic_slice(a,(start,*[0]*(a.ndim-1),),(size,*a.shape[1:]))

# @functools.partial(jit,static_argnums=(2,))	
def vmap(func,in_axes=0,out_axes=0,axes_name=None):	
	'''
	Vectorize function over input axes of iterables
	Args:
		func (callable): Function that acts on single elements of iterables
		in_axes (int,iterable): Input axes of iterables
		out_axes (int,interable): Output axes of func return
		axes_names (object): hashable Python object used to identify the mapped
			axis so that parallel collectives can be applied.
	Returns:
		vfunc (callable): Vectorized function with signature vfunc(*iterables) = [func(*iterables[axes_in][0]),...,func(*iterables[axes_in][n-1])]
	'''
	return jax.vmap(func,in_axes,out_axes,axes_name)



# @functools.partial(jit,static_argnums=(2,))
def vfunc(funcs,index):	
	'''
	Vectorize indexed functions over operands
	Args:
		funcs (iterable[callable]): Functions that act on  that acts on single elements of iterables
		index (iterable[int]): Iterable of indices of functions to call
	Returns:
		vfunc (callable): Vectorized function with signature vfunc(*iterables) = [func(*iterables[axes_in][0]),...,func(*iterables[axes_in][n-1])]
	'''
	# def vfunc(*iterables):
	# 	return array(list(map(lambda i,*x:funcs[i](*x),*[index,*iterables])))
	# return vfunc
	# func = jit(vmap(lambda i,x,funcs=funcs: jax.lax.switch(i,funcs,x)))
	func = lambda index,x: array([funcs[i](x[i]) for i in index])
	return lambda x,func=func,index=index: func(index,x)

@functools.partial(jit,static_argnums=(2,))
def vtensordot(a,b,axes=0):
	'''
	Tensordot product of arrays a and b
	Args:
		a (array): Array to multiply
		b (array): Array to multiply
		axes (int,iterable): axes to perform tensordot 
	Returns:
		out (array): Dot product of array along axes
	'''
	return vmap(lambda a: tensordot(a,b,axes))(a)

@functools.partial(jit,static_argnums=(2,))
def tensordot(a,b,axes=0):
	'''
	Tensordot product of arrays a and b
	Args:
		a (array): Array to multiply
		b (array): Array to multiply
		axes (int,iterable): axes to perform tensordot 
	Returns:
		out (array): Dot product of array along axes
	'''
	return jnp.tensordot(a,b,axes)


@jit
def tensorprod(a):
	'''
	Tensor (kronecker) product of arrays a
	Args:
		a (iterable): Array to perform kronecker product
	Returns:
		out (array): Kronecker product of array
	'''
	out = a[0]
	for i in range(1,len(a)):
		out = jnp.kron(out,a[i])
	return out
	# return jax.lax.fori_loop(1,len(a),lambda i,out: jnp.kron(out,a[i]),a[0])	

@jit
def vtensorprod(a):
	'''
	Tensor (kronecker) product of arrays a
	Args:
		a (iterable): Array to perform kronecker product
	Returns:
		out (array): Kronecker product of array
	'''
	return vmap(tensorprod)(a)



@jit
def ntensorprod(a,n):
	'''
	Tensor (kronecker) product of arrays a
	Args:
		a (iterable): Array to perform kronecker product
		n (int): Perform kronecker product n times
	Returns:
		out (array): Kronecker product of array
	'''
	out = a
	for i in range(1,n):
		out = jnp.kron(out,a)
	return out
	# return jax.lax.fori_loop(1,n,lambda i,out: jnp.kron(out,a),a)	

@jit
def vntensorprod(a,n):
	'''
	Tensor (kronecker) product of arrays a
	Args:
		a (iterable): Array to perform kronecker product
		n (int): Perform kronecker product n times
	Returns:
		out (array): Kronecker product of array
	'''
	return vmap(lambda a: ntensorprod(a,n))(a)


@jit
def einsum(a,subscripts):
	'''
	Get summation of axes in array denoted by subscripts
	Args:
		a (iterable): Arrays to compute summation of elements
		subscripts (str): operations to perform for summation
	Returns:
		out (array): Reduced array of summed array
	'''
	optimize = jnp.einsum_path(subscripts)
	return jnp.einsum(*a,optimize=optimize)


@jit
def contract(array,site,diagonal,size,N,D):
	'''
	Contract product of tensors
	Args:
		array (iterable): Arrays to compute product of arrays
		site (iterable): Subspaces where arrays act
		diagonal (iterable): Tensors which are diagonal		
		size (int): Number of tensors
		N (int): Number of subspaces
		D (int): Dimension of subspaces
	Returns:
		out (array): Reduced array of matrix product of arrays
	'''
	# D = 
	# K = array.size
	# I = eye(self.D**(self.N-K),dtype=int)
	# operator = tensorprod([I,operator])
	# S = contract([swap(i,j,N,D) for i,j in enumerate(sorted(site))],self.site)
	return multi_matmul(array)


@jit
def summation(array,site,diagonal,size,N,D):
	'''
	Add sum of tensors
	Args:
		array (iterable): Arrays to compute sum of arrays
		site (iterable): Subspaces where arrays act
		diagonal (iterable): Tensors which are diagonal		
		size (int): Number of tensors
		N (int): Number of subspaces
		D (int): Dimension of subspaces
	Returns:
		out (array): Reduced array of matrix sum of arrays
	'''
	# D = 
	# K = array.size
	# I = eye(self.D**(self.N-K),dtype=int)
	# operator = tensorprod([I,operator])
	# S = contract([swap(i,j,N,D) for i,j in enumerate(sorted(site))],self.site)
	return multi_add(array)


@jit
def product(array,site,diagonal,size,N,D):
	'''
	Tensor product of tensors
	Args:
		array (iterable): Arrays to tensor product of arrays
		site (iterable): Subspaces where arrays act
		diagonal (iterable): Tensors which are diagonal		
		size (int): Number of tensors
		N (int): Number of subspaces
		D (int): Dimension of subspaces
	Returns:
		out (array): Reduced array of matrix tensor product of arrays
	'''
	# D = 
	# K = array.size
	# I = eye(self.D**(self.N-K),dtype=int)
	# operator = tensorprod([I,operator])
	# S = contract([swap(i,j,N,D) for i,j in enumerate(sorted(site))],self.site)
	return tensorprod(array)


@functools.partial(jit,static_argnums=(1,))
def exponentiate(array,p,site,diagonal,size,N,D):
	r'''
	Perform p-order trotterization of a matrix exponential U = e^{A} ~ f_p({U_i}) + O(|A|^p)
	where f_p is a function of the matrix exponentials {U_i = e^{A_i}} of the 
	k internally commuting components {A_i}  of the matrix A = \sum_i A_i .
	For example, for {U_i = e^{A_i}} :
		f_0 = e^{\sum_i A_i}
		f_1 = \prod_i U_i
		f_2 = \prod_i U_i^{1/2} \prod^i U_i^{1/2}
	For p>0, it will be checked if A_i objects have a matrix exponential module for efficient exponentials,
	otherwise the standard expm function will be used.

	Args:
		array (iterable): Matrix exponential of Tensors
		p (int): Order of trotterization p>0
		site (iterable): Subspaces where arrays act
		diagonal (iterable): Tensors which are diagonal
		size (int): Number of tensors
		N (int): Number of subspaces
		D (int): Dimension of subspaces
	Returns:
		out (array): Trotterized matrix exponential
	'''

	if p == 1:
		array = contract(array,site,diagonal,size,N,D)
	elif p == 2:
		array = contract(asarray([*array[::1],*array[::-1]]),site,diagonal,size,N,D)
	else:
		array = contract(array,site,diagonal,size,N,D)
	return array


@jit
def multi_add(a):
	'''
	Add list of arrays elementwise
	Args:
		a (iterable): Arrays to add
	Returns:
		out (ndarray) if out argument is not None
	'''
	return a.sum(0)
	# return jax.lax.fori_loop(1,len(a),lambda i,out: out+a[i],a[0])


@jit
def multi_matmul(a):
	'''
	Get matrix product of iterable of arrays [a_i], where a_{i}.shape[-1] == a_{i+1}.shape[0]
	Args:
		a (iterable): Arrays to compute product of arrays
	Returns:
		out (array): Reduced array of matrix product of arrays
	'''
	# while len(a) > 1:
	# 	a = matmul(a[::2,...], a[1::2,...])
	# return a
	return jax.lax.fori_loop(1,len(a),lambda i,out: out.dot(a[i]),a[0])

@jit
def multi_dot(a):
	'''
	Get matrix product of iterable of arrays [a_i], where a_{i}.shape[-1] == a_{i+1}.shape[0] with optimized ordering of products
	Args:
		a (iterable): Arrays to compute product of arrays
	Returns:
		out (array): Reduced array of matrix product of arrays
	'''
	return jnp.linalg.multi_dot(a)

@functools.partial(jit,static_argnums=(1,))
def prod(a,axes=0):
	'''
	Get product of elements in array along axes
	Args:
		a (array): Array to compute product of elements
		axes (int): axes to perform product
	Returns:
		out (array): Reduced array of product of elements along axes
	'''
	return jnp.prod(a,axes)


@jit
def matmul(a,b):
	'''
	Calculate matrix product arrays a and b
	Args:
		a (array): Array to calculate matrix product
		b (array): Array to calculate matrix product
	Returns:
		out (array): Matrix product
	'''	
	return jnp.matmul(a,b)


@jit
def commutator(a,b):
	'''
	Calculate commutator of ab - ba
	Args:
		a (array): Array to calculate commutator
		b (array): Array to calculate commutator
	Returns:
		out (array): commutator
	'''	
	return tensordot(a,b,1) - tensordot(b,a,1)


# @jit
def commutes(a,b):
	'''
	Calculate whether a and b commutate
	Args:
		a (array): Array to calculate commutator
		b (array): Array to calculate commutator
	Returns:
		commutes (bool): Boolean of a and b commuting
	'''	
	return bool(~(commutator(a,b).any()))

@jit
def anticommutator(a,b):
	'''
	Calculate anticommutator of ab + ba
	Args:
		a (array): Array to calculate anticommutator
		b (array): Array to calculate anticommutator
	Returns:
		out (array): anticommutator
	'''	
	return tensordot(a,b,1) + tensordot(b,a,1)


@jit
def anticommutes(a,b):
	'''
	Calculate whether a and b anticommutate
	Args:
		a (array): Array to calculate anticommutator
		b (array): Array to calculate anticommutator
	Returns:
		commutes (bool): Boolean of a and b anticommuting
	'''	
	return bool(~(anticommutator(a,b).any()))


@jit
def inner(a,b):
	'''
	Calculate inner product of arrays a and b
	Args:
		a (array): Array to calculate inner product
		b (array): Array to calculate inner product
	Returns:
		out (array): Inner product
	'''	
	return abs(trace(tensordot(a,b,1)))


@jit
def gradinner(a,b,c,d,x,y):
	'''
	Calculate gradient inner product of arrays a and b with respect to a = e^(c(x)), b = e^(d(y))
	Args:
		a (array): Array to calculate inner product
		b (array): Array to calculate inner product
		c (array): Array to calculate inner product
		d (array): Array to calculate inner product
		x (array): Array to calculate inner product
		y (array): Array to calculate inner product
	Returns:
		out (array): Inner product gradient
	'''	
	return (1/2)*(1/inner(a,b))*(trace(tensordot(a.conj().T,b@d/y,1))*trace(tensordot(a.T,b.conj(),1)) + 
								  trace(tensordot(a.conj().T,b,1))*trace(tensordot(a.T,(b@d/y).conj().T,1)))

@jit
def trace(a):
	'''
	Calculate trace of array
	Args:
		a (array): Array to calculate trace
	Returns:
		out (array): Trace of array
	'''	
	return jnp.trace(a)

@jit
def abs(a):
	'''
	Calculate absolute value of array
	Args:
		a (array): Array to calculate absolute value
	Returns:
		out (array): Absolute value of array
	'''	
	return jnp.abs(a)



@jit
def eigh(a):
	'''
	Compute eigenvalues and eigenvectors of a hermitian array
	Args:
		a (array): Array to compute eigenvalues and eigenvectors of shape (...,n,n)
	Returns:
		e (array): Vector of eigenvalues of shape (...,n)
		v (array): Array of normalized eigenvectors of shape (...,n,n)
	'''
	return jnp.linalg.eigh(a)
	



@jit
def sqrtm(a):
	'''
	Calculate matrix square-root of array a
	Args:
		a (array): Array to compute square root
	Returns:
		out (array): Square root of array
	'''
	return jnp.sqrtm(a)


@jit
def sqrt(a):
	'''
	Calculate square-root of array a
	Args:
		a (array): Array to compute square root
	Returns:
		out (array): Square root of array
	'''
	return jnp.sqrt(a)


@jit
def log(a):
	'''
	Calculate natural log of array a
	Args:
		a (array): Array to compute log
	Returns:
		out (array): Natural log of array
	'''
	return jnp.log(a)

@jit
def exp(a):
	'''
	Calculate element-wise exponential of array a
	Args:
		a (array): Array to compute element-wise exponential
	Returns:
		out (array): Element-wise exponential of array
	'''
	return jnp.exp(a)

@jit
def expm(a):
	'''
	Calculate matrix exponential of array a
	Args:
		a (array): Array to compute matrix exponential
	Returns:
		out (array): Matrix exponential of array
	'''
	return jsp.linalg.expm(a)


@jit
def vexpm(a):
	'''
	Calculate matrix exponential of array a
	Args:
		a (array): Array to compute matrix exponential
	Returns:
		out (array): Matrix exponential of array
	'''
	return vmap(expm)(a)




@jit
def expeuler(a,x,I):
	'''
	Calculate element-wise exponential of array x*a using euler identity (assuming a^2 = I)
	Args:
		a (array): Array to compute element-wise exponential
		x (array): Coefficients for exponential
		I (array): Array of identity of shape of a
	Returns:
		out (array): Element-wise exponential of array
	'''
	return cosh(x)*I + sinh(x)*a

@jit
def expmeuler(a,x,I):
	'''
	Calculate matrix exponential of array a using euler identity (assuming a^2 = I)
	Args:
		a (array): Array to compute matrix exponential
		x (array): Coefficients for exponential
		I (array): Array of identity of shape of a		
	Returns:
		out (array): Matrix exponential of array
	'''
	return cosh(x)*I + sinh(x)*a


@jit
def vexpmeuler(a,x,I):
	'''
	Calculate matrix exponential of array {a_i} using euler identity (assuming a_i^2 = I)
	Args:
		a (array): Array to compute matrix exponential
		x (array): Coefficients for exponential
		I (array): Array of identity of shape of a elements	a_i	
	Returns:
		out (array): Matrix exponential of array
	'''
	return vmap(expmeuler,(0,0,None))(a,x,I)

@jit
def sin(a):
	'''
	Calculate sine of array a
	Args:
		a (array): Array to compute sine
	Returns:
		out (array): Sine of array
	'''
	return jnp.sin(a)


@jit
def cos(a):
	'''
	Calculate cosine of array a
	Args:
		a (array): Array to compute cosine
	Returns:
		out (array): Cosine of array
	'''
	return jnp.cos(a)


@jit
def sinh(a):
	'''
	Calculate sinh of array a
	Args:
		a (array): Array to compute sinh
	Returns:
		out (array): Sinh of array
	'''
	return jnp.sinh(a)


@jit
def cosh(a):
	'''
	Calculate cosinh of array a
	Args:
		a (array): Array to compute cosinh
	Returns:
		out (array): Cosinh of array
	'''
	return jnp.cosh(a)


@functools.partial(jit,static_argnums=(1,))
def concatenate(a,axis):
	'''
	Concatenate iterables along axis
	Args:
		a (iterable): Iterables to concatenate
		axis (int): Axis to concatenate arrays
	Returns:
		out (iterable): Concatenation row-wise
	'''
	return jnp.concatenate(a,axis)


@jit
def diag(a):
	'''
	Get diagonal of array
	Args:
		a (array): Array to get diagonal
	Returns:
		out (array): Diagonal of array
	'''
	return jnp.diag(a)

@jit
def mod(a,b):
	'''
	Element wise modular division of a mod b
	Args:
		a (array): Array to compare
		b (array): Array to divide by
	Returns:
		out (array): Modular division of a mod b
	'''
	return jnp.mod(a,b)


# @functools.partial(jit,static_argnums=(1,))
def unique(a,axis=None):
	'''
	Find unique elements of array
	Args:
		a (array): Array to search for unique elements
		axis (int): Axis to search for unique elements
	Returns:
		out (array): Unique elements of array
	'''
	return jnp.unique(a,axis=axis)

def uniqueobjs(a,axis=None):
	'''
	Find unique elements of array
	Args:
		a (array): Array to search for unique elements
		axis (int): Axis to search for unique elements
	Returns:
		out (array): Unique elements of array
	'''
	return np.unique(a,axis=axis)


@functools.partial(jit,static_argnums=(1,2,))
def repeat(a,repeats,axis):
	'''
	Repeat array repeats-times along axis
	Concatenate iterables row-wise
	Args:
		a (array): Array to repeat
		repeats (int): Number of times to repeat
		axis (int): Axis along which to repeat
	Returns:
		out (array): Repeated array
	'''
	return jnp.repeat(a,repeats,axis)



@functools.partial(jit,static_argnums=(0,1,2,))
def randomstring(K,N,D=2):
	'''
	Create K random N-qudit Pauli string arrays of shape (K,D**N,D**N)
	Args:
		K (int): Number of Pauli strings
		N (int): Number of qudits
		D (int): Dimension of qudits
	Returns:
		string (array): Pauli string arrays of shap (K,D**N,D**N)
	'''
	
	assert D==2,"Qudits for D=%d > 2 not implemented"%(D)
	
	seed = np.random.randint(1e6)
	key = PRNGKey(seed)

	d = int(D**2)
	if D == 2:
		basis = array([[[1,0],[0,1]],[[0,1],[1,0]],[[0,-1j],[1j,0]],[[1,0],[0,-1]]])
	else:
		basis = array([[[1,0],[0,1]],[[0,1],[1,0]],[[0,-1j],[1j,0]],[[1,0],[0,-1]]])

	alpha = jax.random.uniform(key,(K*N,d))
	
	string = vtensordot(alpha,basis,1)
	string = string.reshape((K,N,D,D))
	string = vtensorprod(string)
	
	return string
	


@functools.partial(jit,static_argnums=(0,1,2,))
def paulistring(string,N,K,D=2):
	'''
	Create K N-qudit Pauli string arrays of shape (K,D**N,D**N)
	Args:
		K (int): Number of Pauli strings
		N (int): Number of qudits
		D (int): Dimension of qudits
	Returns:
		string (array): Pauli string arrays of shap (K,D**N,D**N)
	'''
	
	assert D==2,"Qudits for D=%d > 2 not implemented"%(D)
	
	seed = np.random.randint(1e6)
	key = PRNGKey(seed)

	d = int(D**2)
	if D == 2:
		basis = array([[[1,0],[0,1]],[[0,1],[1,0]],[[0,-1j],[1j,0]],[[1,0],[0,-1]]])
	else:
		basis = array([[[1,0],[0,1]],[[0,1],[1,0]],[[0,-1j],[1j,0]],[[1,0],[0,-1]]])

	alpha = jax.random.uniform(key,(K*N,d))
	
	string = vtensordot(alpha,basis,1)
	string = string.reshape((K,N,D,D))
	string = vtensorprod(string)
	
	return string
	



@functools.partial(jit,static_argnums=(2,3,4,))
def allclose(a,b,rtol=1e-05,atol=1e-08,equal_nan=False):
	'''
	Check if arrays a and b are all close within tolerance
	Args:
		a (array): Array to compare with
		b (array): Array to compare
		rtol (float): Relative tolerance of arrays
		atol (float): Absolute tolerance of arrays
		equal_nan (bool): Compare nan's as equal
	Returns:
		out (bool): Boolean of whether a and b are all close
	'''
	return jnp.allclose(a,b,rtol,atol,equal_nan)	


@functools.partial(jit,static_argnums=(2,3,4,))
def isclose(a,b,rtol=1e-05,atol=1e-08,equal_nan=False):
	'''
	Check if arrays a and b are close within tolerance
	Args:
		a (array): Array to compare with
		b (array): Array to compare
		rtol (float): Relative tolerance of arrays
		atol (float): Absolute tolerance of arrays
		equal_nan (bool): Compare nan's as equal
	Returns:
		out (array): Boolean of whether a and b are close
	'''
	return jnp.isclose(a,b,rtol,atol,equal_nan)	



def isiterable(obj):
	'''
	Check if object is iterable
	Args:
		obj (object): object to be tested
	Returns:
		iterable (bool): whether object is iterable
	'''
	return hasattr(obj,'__iter__')


@jit
def isdiag(a):
	'''
	Check if array is diagonal
	Args:
		a (array): Possible diagonal array of shape (n,n)
	Returns:
		iterable (bool): whether object is iterable
	'''	
	n,m = a.shape
	assert n == m
	return ~a.ravel()[:-1].reshape(n-1,m+1)[:,1:].any()


def PRNG(seed,split=False):
	'''
	Generate PRNG key
	Args:
		seed (int,array): Seed for random number generation or random key for future seeding
	Returns:
		key (array): Random key
		subkey (array): Random array (if split is True)
	'''	
	if seed is None:
		seed = 0
	if isinstance(seed,(int)):
		key = jax.random.PRNGKey(seed)
	else:
		key = seed
	if split:
		key,subkey = jax.random.split(key)	
		return key,subkey
	else:
		return key



@functools.partial(jit,static_argnums=(0,1))
def finitegrad(func,eps=1e-5):
	'''
	Calculate finite difference second order derivative of function
	Args:
		func (callable): Function to derive, with signature func(x) and output shape
	Returns:
		out (array): Array of gradient
	'''
	return lambda x: vmap(lambda v: (func(x+eps*v)-func(x-eps*v))/(2*eps))(eye(x.size))


@functools.partial(jit,static_argnums=(0,1))
def value_and_finitegrad(func,eps=1e-5):
	'''
	Calculate finite difference second order derivative of function
	Args:
		func (callable): Function to derive, with signature func(x) and output shape
	Returns:
		out (array): Array of gradient
	'''
	return lambda x: (func(x),vmap(lambda v: (func(x+eps*v)-func(x-eps*v))/(2*eps))(eye(x.size)))



# @functools.partial(jit,static_argnums=(0,))
def grad(func):
	'''
	Calculate auto-differentiation derivative of function
	Args:
		func (callable): Function to derive, with signature func(x) and output shape
	Returns:
		out (array): Array of shape (n,*shape)
	'''
	return jax.grad(func)


# @functools.partial(jit,static_argnums=(0,))
def value_and_grad(func):
	'''
	Calculate auto-differentiation derivative of function
	Args:
		func (callable): Function to derive, with signature func(x) and output shape
	Returns:
		out (array): Array of shape (n,*shape)
	'''
	return jax.value_and_grad(func)


# Get if array is sparse
def issparse(arr,*args,**kwargs):
	return issparsematrix(arr) or issparsearray(arr)

# Get if array is sparse matrix
def issparsematrix(arr,*args,**kwargs):
	return sp.sparse.issparse(arr)

# Get if array is sparse array
def issparsearray(arr,*args,**kwargs):
	return isinstance(arr,sparray.SparseArray)

# Get if array is numpy array
def isndarray(arr,*args,**kwargs):
	return isinstance(arr,(np.ndarray))

# Get if array is pandas dataframe
def isdataframe(arr,*args,**kwargs):
	return isinstance(arr,(pd.DataFrame))

# Get if array is array
def isarray(arr,*args,**kwargs):
	return isndarray(arr) or issparse(arr)

# Get if array is scalar
def isscalar(arr,*args,**kwargs):
	return (not isarray(arr) and not islisttuple(arr)) or (isarray(arr) and (arr.ndim<1) and (arr.size<2))

# Get if array is None
def isnone(arr,*args,**kwargs):
	return arr is None

# Get if array is python list
def islist(arr,*args,**kwargs):
	return isinstance(arr,(list))

# Get if array is python tuple
def istuple(arr,*args,**kwargs):
	return isinstance(arr,(tuple))

# Get if array is python list,tuple
def islisttuple(arr,*args,**kwargs):
	return islist(arr) or istuple(arr)


def flattener(iterable,notiterable=(str,)):
	'''
	Flatten iterable up to layer layers deep into list
	Args:
		iterable (iterable): object to flatten
		notiterable (tuple): object types not to flatten
	Returns:
		flat (generator): generator of flattened iterable
	'''
	for i in iterable:
		if isiterable(i) and not isinstance(i,notiterable):
			for j in flattener(i,notiterable):
				yield j
		else:
			yield i


def flatten(iterable,cls=list,notiterable=(str,),unique=False):
	'''
	Flatten iterable into cls object
	Args:
		iterable (iterable): object to flatten
		cls (class): Class to initialize with flattened iterable
		notiterable (tuple): object types not to flatten		
		unique (bool): Return only unique elements from flattened iterable
	Returns:
		flat (cls): instance of flattened iterable
	'''	
	uniquecls = set if unique else list
	return cls(list(uniquecls(flattener(iterable,notiterable))))	


def parse(string,dtype):
	'''
	Parse string as numerical type
	Args:
		string (str): String to aprse
		dtype (callable): Type to convert to
	Returns:
		parsed (object): Converted string
	'''
	parsed = string
	try:
		parsed = dtype(string)
	except:
		pass
	return parsed


def returnargs(returns):
	'''
	Return tuple of returns, returning first element if tuple and length 1 iterable
	Args:
		returns (iterable): Iterable of return values
	Returns:
		returns (iterable,object): Return values
	'''

	if isinstance(returns,tuple) and len(returns) == 1:
		return returns[0]
	else:
		return returns


def generator(stop=None):
	'''
	Generator wrapper to restart stop number of times
	'''
	def wrap(func):
		def set(*args,**kwargs):
			return func(*args,*kwargs)
		@functools.wraps(func)
		def wrapper(*args,stop=stop,**kwargs):
			generator = set(*args,**kwargs)
			while stop:
				try:
					yield next(generator)
				except StopIteration:
					stop -= 1
					generator = set(*args,**kwargs)
					yield next(generator)
		return wrapper
	return wrap



def copier(key,value,_copy):
	'''
	Copy value based on associated key 

	Args:
		key (string): key associated with value to be copied
		value (python object): data to be copied
		_copy (bool,dict,None): boolean or None whether to copy value, or dictionary with keys on whether to copy value
	Returns:
		Copy of value
	'''

	# Check if _copy is a dictionary and key is in _copy and is True to copy value
	if ((not _copy) or (isinstance(_copy,dict) and (not _copy.get(key)))):
		return value
	else:
		return copy.deepcopy(value)

def permute(dictionary,_copy=False,_groups=None,_ordered=True):
	'''
	Get all combinations of values of dictionary of lists

	Args:
		dictionary (dict): dictionary of keys with lists of values to be combined in all combinations across lists
		_copy (bool,dict,None): boolean or None whether to copy value, or dictionary with keys on whether to copy value
		_groups (list,None): List of lists of groups of keys that should not have their values permuted in all combinations, but should be combined in sequence element wise. For example groups = [[key0,key1]], where dictionary[key0] = [value_00,value_01,value_02],dictionary[key1] = [value_10,value_11,value_12], then the permuted dictionary will have key0 and key1 keys with only pairwise values of [{key0:value_00,key1:value_10},{key0:value_01,key1:value_11},{key0:value_02,key1:value_12}].
		_ordered (bool): Boolean on whether to return dictionaries with same ordering of keys as dictionary

	Returns:
		List of dictionaries with all combinations of lists of values in dictionary
	'''		
	def indexer(keys,values,_groups):
		'''
		Get lists of values for each group of keys in _groups
		'''
		_groups = copy.deepcopy(_groups)
		if _groups is not None:
			inds = [[keys.index(k) for k in g if k in keys] for g in _groups]
		else:
			inds = []
			_groups = []
		N = len(_groups)
		_groups.extend([[k] for k in keys if all([k not in g for g in _groups])])
		inds.extend([[keys.index(k) for k in g if k in keys] for g in _groups[N:]])
		values = [[values[j] for j in i ] for i in inds]
		return _groups,values

	def zipper(keys,values,_copy): 
		'''
		Get list of dictionaries with keys, based on list of lists in values, retaining ordering in case of grouped values
		'''
		return [{k:copier(k,u,_copy) for k,u in zip(keys,v)} for v in zip(*values)]

	def unzipper(dictionary):
		'''
		Zip keys of dictionary of list, and values of dictionary as list
		'''
		keys, values = zip(*dictionary.items())	
		return keys,values

	def permuter(dictionaries): 
		'''
		Get all list of dictionaries of all permutations of sub-dictionaries
		'''
		return [{k:d[k] for d in dicts for k in d} for dicts in itertools.product(*dictionaries)]

	def nester(keys,values):
		'''
		Get values of permuted nested dictionaries in values.
		Recurse permute until values are lists and not dictionaries.
		'''
		keys,values = list(keys),list(values)
		for i,(key,value) in enumerate(zip(keys,values)):
			if isinstance(value,dict):
				if isinstance(_groups,dict):
					_group = _groups.get(key,_group)
				else:
					_group = _groups
				values[i] = permute(value,_copy=_copy,_groups=_group)    
		return keys,values


	if dictionary in [None,{}]:
		return [{}]

	# Get list of all keys from dictionary, and list of lists of values for each key
	keys,values = unzipper(dictionary)


	# Get values of permuted nested dictionaries in values
	keys,values = nester(keys,values)

	# Retain ordering of keys in dictionary
	keys_ordered = keys
	
	# Get groups of keys based on _groups and get lists of values for each group
	keys,values = indexer(keys,values,_groups)

	# Zip keys with lists of lists in values into list of dictionaries
	dictionaries = [zipper(k,v,_copy) for k,v in zip(keys,values)]


	# Get all permutations of list of dictionaries into one list of dictionaries with all keys
	dictionaries = permuter(dictionaries)


	# Retain original ordering of keys if _ordered is True
	if _ordered:
		for i,d in enumerate(dictionaries):
			dictionaries[i] = {k: dictionaries[i][k] for k in keys_ordered}    
	return dictionaries


@functools.partial(jit,static_argnums=(1,2,))
def binary(a,n,function):
	'''
	Perform binary function recursively on n-fold reduction of n arguments f(1(f(2,...f(n-1,n))
	Args:
		a (iterable): iterable of length n of arguments
		n (int): Number of reductions to perform
		function (callable): Binary function with signature function(x,y)
	Returns:
		out (object): Reduced output of binary function
	'''
	if n == 1:
		return a[0]
	elif n == 2:
		return function(a[0],a[1])
	else:        
		m = n%2
		n = n//2
		if m:
			return function(a[0],
				   function(binary(a[m:n+m],n,function),
							binary(a[n+m:],n,function)))
		else:
			return function(binary(a[m:n+m],n,function),
							binary(a[n+m:],n,function))   



@functools.partial(jit,static_argnums=(2,))
def trotter(A,U,p):
	r'''
	Perform p-order trotterization of a matrix exponential U = e^{A} ~ f_p({U_i}) + O(|A|^p)
	where f_p is a function of the matrix exponentials {U_i = e^{A_i}} of the 
	k internally commuting components {A_i}  of the matrix A = \sum_i^k A_i .
	For example, for {U_i = e^{A_i}} :
		f_0 = e^{\sum_i A_i}
		f_1 = \prod_i^k U_i
		f_2 = \prod_i^k U_i^{1/2} \prod_k^i U_i^{1/2}
	For p>0, it will be checked if A_i objects have a matrix exponential module for efficient exponentials,
	otherwise the standard expm function will be used.

	Args:
		A (iterable): Array of shape (k,n,n) of k components of a square matrix of shape (n,n) A_i	
		U (iterable): Array of shape (k,n,n) of k components of the matrix exponential of a square matrix of shape (n,n) expm(A_i/p)
		p (int): Order of trotterization p>0
	Returns:
		U (array): Trotterized matrix exponential of shape (n,n)
	'''
	if p == 1:
		U = multi_matmul(U)
	elif p == 2:
		U = multi_matmul(asarray([*U[::1],*U[::-1]]))
	else:
		U = multi_matmul(U)
	return U


# @functools.partial(jit,static_argnums=(1,2,3,))
@functools.partial(jit,static_argnums=(2,))
def trottergrad(A,U,p):
	r'''
	Perform gradient of p-order trotterization of a matrix exponential U = e^{A} ~ f_p({U_i}) + O(|A|^p)
	where f_p is a function of the matrix exponentials {U_i = e^{A_i}} of the 
	k internally commuting components {A_i}  of the matrix A = \sum_i^k A_i .
	For example, for {U_i = e^{A_i}} :
		f_0 = e^{\sum_i A_i}
		f_1 = \prod_i^k U_i
		f_2 = \prod_i^k U_i^{1/2} \prod_k^i U_i^{1/2}
	For p>0, it will be checked if A_i objects have a matrix exponential module for efficient exponentials,
	otherwise the standard expm function will be used.

	Args:
		A (iterable): Array of shape (k,n,n) of k components of a square matrix of shape (n,n) A_i
		U (iterable): Array of shape (k,n,n) of k components of the matrix exponential of a square matrix of shape (n,n) expm(A_i/p)
		p (int): Order of trotterization p>0
	Returns:
		U (array): Gradient of Trotterized matrix exponential of shape (k,n,n)
	'''
	k = len(U)
	if p == 1:
		U = array([multi_matmul(asarray([*U[:i],A[i]/p,*slices(U,i,k-i)])) for i in range(k)])
	elif p == 2:
		U = array([multi_matmul(asarray([*slices(U,0,i)[::1],A[i]/p,*slices(U,i,k-i)[::1],*U[::-1]])) + 
				   multi_matmul(asarray([*U[::1],*slices(U,i,k-i)[::-1],A[i]/p,*slices(U,0,i)[::-1]]))
				   for i in range(k)])
	else:
		U = array([multi_matmul(asarray([*slices(U,0,i),A[i]/p,*slices(U,i,k-i)])) for i in range(k)])
	return U