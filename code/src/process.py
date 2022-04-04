#!/usr/bin/env python

# Import python modules
import os,sys
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

# Logging
import logging
conf = 'logging.conf'
try: 
	logging.config.fileConfig(conf,disable_existing_loggers=False) 
except: 
	pass
logger = logging.getLogger(__name__)


# Import user modules
ROOT = os.path.dirname(os.path.abspath(__file__))
PATHS = ['','..','../..','../../lib']
for PATH in PATHS:
	sys.path.append(os.path.abspath(os.path.join(ROOT,PATH)))

from lib.utils.io import setup,load,dump,_load
from lib.utils.plot import plot

def texify(string):
	strings = {
		'N':r'$N$'%(),
		'h':r'$h/J$'%(),
		'order':r'$\sigma/N$'%(),
		'energy':r'$\epsilon/N$'%(),
		'gap':r'$\Delta/N$'%(),
	}
	if not isinstance(string,str):
		string = str(string)

	default = r'$%s$'%(string.replace('$',''))

	return strings.get(string,default)

def main(args):

	path = args[0]
	directory = os.path.dirname(path)
	wr = 'r'
	default = {}
	kwargs = {}

	data = load(path,wr=wr,default=default,**kwargs)

	X,Y,Z,I = np.array([[0,1],[1,0]]),np.array([[0,-1j],[1j,0]]),np.array([[1,0],[0,-1]]),np.array([[1,0],[0,1]])
	def tensorprod(a):
		out = a[0]
		for i in range(1,len(a)):
			out = np.kron(out,a[i])
		return out

	# for name in data:
	# 	N = data[name]['N']
	# 	D = data[name]['D']
	# 	J = data[name]['J']
	# 	h = data[name]['h']
	# 	e = data[name]['energy']
	# 	o = data[name]['order']


	# 	if N >= 12 or h>0.5:
	# 		continue

	# 	print(N,h)
	# 	H = [
	# 		*[[(1j*(J)**(1./2))*Z if k in [i,j] else I for k in range(N)] for i in range(N) for j in [(i+1)%N]],
	# 		*[[(-(h)**(1./1))*X if k in [i] else I  for k in range(N)] for i in range(N)]
	# 		]
	# 	H = np.sum([tensorprod(u).real for u in H],0)



	# 	eigenvalues,eigenvectors = np.linalg.eigh(H)

	# 	eigenvalues = np.unique(eigenvalues.round(10))

	# 	assert all(np.allclose(eigenvalues[i]/N,e[i]) for i in range(e.size)), "%r, %r"%(eigenvalues[:3]/N,e);

	# 	print(eigenvalues[:3]/N)
	# 	print()






	attrs = list(set([attr for name in data for attr in data[name]]))

	parameters = {attr: list(sorted(set([data[name][attr] 
					for name in data if attr in data[name] and isinstance(data[name][attr],(int,np.integer,float,np.float))]))) 
			for attr in attrs}
	values = {attr: [data[name][attr] for name in data if attr in data[name] and not isinstance(data[name][attr],(int,np.integer,float,np.float))] 
			for attr in attrs}
	parameters = {attr: parameters[attr] for attr in parameters if len(parameters[attr])>0}
	values = {attr: values[attr] for attr in values if len(values[attr])>0}
	
	sizes = {attr: min([data[name][attr].size for name in data]) for attr in values}






	xattrs = ['h','h','N']
	yattrs = ['energy','order','gap']
	iattrs = ['N','N','h']

	x = {(xattr,yattr,iattr): 
		{j: {i: np.array([data[name][xattr] for name in data if data[name][iattr] == i]) 
		for i in parameters[iattr]} 
		for j in range(sizes[yattr])}
		for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs)
		}
	y = {(xattr,yattr,iattr): 
		{j:{i: np.array([data[name][yattr][j] for name in data if data[name][iattr] == i]) 
		for i in parameters[iattr]} 
		for j in range(sizes[yattr])}
		for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs)
		}

	xfuncs = {(xattr,yattr,iattr): 
		{j:{i: (lambda x,y,xattr=xattr,yattr=yattr,iattr=iattr,i=i: (
			{'h': x/np.array([data[name]['J'] for name in data if data[name][iattr] == i]),
			 'N': 1/x if xattr =='N' else x,
			}[xattr]))
		for i in parameters[iattr]} 
		for j in range(sizes[yattr])}
		for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs)
		}

	yfuncs = {(xattr,yattr,iattr): 
		{j:{i: (lambda x,y,xattr=xattr,yattr=yattr,iattr=iattr,i=i: (
			{'energy': y,
			 'order':y,
			 'gap': y,
			}[yattr]))
		for i in parameters[iattr]} 
		for j in range(sizes[yattr])}
		for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs)
		}		

	
	argsort = {key: {j: {i: np.argsort(x[key][j][i]) for i in x[key][j]} for j in x[key]} for key in x}
	x = {key: {j: {i: x[key][j][i][argsort[key][j][i]] for i in x[key][j]} for j in x[key]} for key in x}
	y = {key: {j: {i: y[key][j][i][argsort[key][j][i]] for i in y[key][j]} for j in y[key]} for key in y}

	subplots = False

	settings = {
		key:{
		j: {
			'ax': {
				'plot':[{
					'x': xfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'y': yfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'marker':['o','^','*'][j] if subplots else 'o',
					'color':getattr(plt.cm,'tab10')(l) if (key[1] != 'gap') else getattr(plt.cm,'viridis')(l/len(x[key][j])),
					'label':texify(i) if j==(0) else None,
					}
					for l,i in enumerate(x[key][j])
					],
				'set_title':{'label':r'$\textrm{Level %d}$'%(j)},
				'set_xlabel':{'xlabel':texify(key[0]) if (key[1] != 'gap') else texify(r'1/%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear' if (key[1] != 'gap') else 'log'},				
				# 'set_yscale':{'value':'linear' if (key[1] != 'gap') else 'log'},				
				**({'set_xticks':{'ticks':[0,1,2]}} if (key[1] != 'gap') else {}),
				'legend':{'title':texify(key[2]),'ncol':1 if (key[1] != 'gap') else 4},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{
					'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2])),
				'close':{'fig':True},
				},
			},
			'style':{
				# 'texify':texify,
				# 'mplstyle':settings['plot']['mplstyle'][name],	
				# 'rcParams':{'font.size':settings['plot']['settings'][key][name]['other']['fontsize']},						        		
				'layout':{
					'nrows':1,
					'ncols':1,
					'index':1,
				} if subplots or (key[1] == 'gap') else
				{
					'nrows':1,
					'ncols':sizes[key[1]],
					'index':j+1,
				},
			},			
		}
	for j in (range(sizes[key[1]]) if key[1] != 'gap' else [0])}
	for k,key in enumerate(x)
	}




	for k,key in enumerate(x):
		plot(settings=settings[key])


if __name__ == '__main__':
	main(sys.argv[1:])