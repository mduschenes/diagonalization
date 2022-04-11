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
		'entanglement':r'$S$'%(),
		'x':r'$L/N$'%(),
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

	attrs = list(set([attr for name in data for attr in data[name]]))

	parameters = {attr: list(sorted(set([data[name][attr] 
					for name in data if attr in data[name] and isinstance(data[name][attr],(int,np.integer,float,np.float))]))) 
			for attr in attrs}
	values = {attr: [data[name][attr] for name in data if attr in data[name] and not isinstance(data[name][attr],(int,np.integer,float,np.float))] 
			for attr in attrs}
	parameters = {attr: parameters[attr] for attr in parameters if len(parameters[attr])>0}
	values = {attr: values[attr] for attr in values if len(values[attr])>0}
	
	sizes = {attr: min([data[name][attr].shape[0] for name in data]) for attr in values}


	xattrs = ['h','h','N','x']
	yattrs = ['energy','order','gap','entanglement']
	iattrs = ['N','N','h','N']

	x = {(xattr,yattr,iattr): 
		{j: {i: np.array([data[name][xattr] for name in data if data[name][iattr] == i]) if xattr in attrs else np.arange(np.array([1 for name in data if data[name][iattr] == i]).size)
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
			 'x': x/np.array([data[name]['N'] for name in data if data[name][iattr] == i]),
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
			 'entanglement':y,
			}[yattr]))
		for i in parameters[iattr]} 
		for j in range(sizes[yattr])}
		for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs)
		}		

	
	argsort = {key: {j: {i: np.argsort(x[key][j][i]) for i in x[key][j]} for j in x[key]} for key in x}
	x = {key: {j: {i: x[key][j][i][argsort[key][j][i]] for i in x[key][j]} for j in x[key]} for key in x}
	y = {key: {j: {i: y[key][j][i][argsort[key][j][i]] for i in y[key][j]} for j in y[key]} for key in y}


	settings = {
		**{
		key:{
		j: {
			'ax': {
				'plot':[{
					'x': xfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'y': yfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'marker':'o',
					'color':getattr(plt.cm,'tab10')(l),
					'label':texify(i) if j==(0) else None,
					}
					for l,i in enumerate(x[key][j])
					],
				'set_title':{'label':r'$\textrm{Level %d}$'%(j)},
				'set_xlabel':{'xlabel':texify(key[0])},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xticks':{'ticks':[0,1,2]},
				'legend':{'title':texify(key[2]),'ncol':1},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2]))},
				# 'close':{'fig':True},
			},
			'style':{
				'layout':{
					'nrows':1,
					'ncols':sizes[key[1]],
					'index':j+1,
				},
			},			
		}
		for j in range(sizes[key[1]])}
		for key in [('h','energy','N'),('h','order','N')]},
		**{
		key:{
		j: {
			'ax': {
				'plot':[{
					'x': xfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'y': yfuncs[key][j][i](x[key][j][i],y[key][j][i],*key,i),
					'marker':'o',
					'color':getattr(plt.cm,'RdYlBu')(l/len(x[key][j])),
					'label':texify(i) if j==(0) else None,
					}
					for l,i in enumerate(x[key][j])
					],
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(j)},
				'set_xlabel':{'xlabel':texify(r'1/%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				# 'legend':{'title':texify(key[2]),'ncol':4},
				'grid':{'visible':True},
				'set_colorbar':{
					'values':[2*(l/len(x[key][j])) for l,i in enumerate(x[key][j])],
					'colors':[getattr(plt.cm,'RdYlBu')(l/len(x[key][j])) for l,i in enumerate(x[key][j])],
					'size':'3%',
					'pad':0.1,
					'set_ylabel':{'ylabel':texify(key[2])},
					}  if j == 0 else {},			
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2]))},
				# 'close':{'fig':True},
			},
			'style':{
				'layout':{
					'nrows':1,
					'ncols':1,
					'index':1,
				}
			},			
		}
		for j in [0]} #range(sizes[key[1]]-1)}
		for key in [('N','gap','h')]},	
	**{
		key:{
		j: {
			'ax': {
				'plot':[{
					'x': np.arange(1,len([data[name][key[1]][j] for name in data if data[name][key[2]] == i][0])+1)/i,
					'y': [data[name][key[1]][j] for name in data if data[name][key[2]] == i][0],
					'marker':'o',
					'color':getattr(plt.cm,'tab10')(l),
					'label':texify(i) if j==(0) else None,
					}
					for l,i in enumerate(parameters[key[2]])
					],
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(j)},
				'set_xlabel':{'xlabel':texify(r'%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xlim':{'xmin':0,'xmax':1},				
				'set_xticks':{'ticks':[0,0.25,0.5,0.75,1]},				
				'legend':{'title':texify(key[2]),'ncol':4},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':12,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2]))},
				# 'close':{'fig':True},
			},
			'style':{
				'layout':{
					'nrows':1,
					'ncols':1,
					'index':1,
				}
			},			
		}
		for j in [0]} #range(sizes[key[1]]-1)}
		for key in [('x','entanglement','N')]},			
	}




	for k,key in enumerate(settings):
		plot(settings=settings[key])


if __name__ == '__main__':
	main(sys.argv[1:])