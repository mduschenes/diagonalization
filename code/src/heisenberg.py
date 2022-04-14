#!/usr/bin/env python

# Import python modules
import os,sys,itertools,warnings
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
warnings.filterwarnings('ignore')

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

def texify(string,usetex=True):
	strings = {
		'N':r'$N$'%(),
		'h':r'$h/J$'%(),
		'U':r'$U$'%(),
		'order':r'$\sigma/N$'%(),
		'energy':r'$\epsilon/N$'%(),
		'gap':r'$\Delta/N$'%(),
		'entanglement':r'$S$'%(),
		'partition':r'$L/N$'%(),
	}
	if not isinstance(string,str):
		string = str(string)

	default = r'$%s$'%(string.replace('$',''))

	string = strings.get(string,default)

	if not usetex:
		string = string.replace('$','')

	return string


def fit(x,y,intercept=True):
	if intercept:
		x = np.array([x,np.ones(x.size)]).T
	else:
		x = np.array([x]).T
	x[np.isnan(x) | np.isinf(x)] = 0
	x[np.abs(x)<0] = 0
	coef = np.linalg.lstsq(x,y)[0] + 0.0
	y = x.dot(coef)
	return y,coef


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
	
	sizes = {attr: min([data[name][attr].shape[0] for name in data]) if attr in values else 1 for attr in attrs}


	xattrs = ['U','U','N','partition','N']
	yattrs = ['energy','order','gap','entanglement','energy']
	iattrs = [('N',),('N',),('U',),('N','U',),('U','J')]

	variables = {}

	for (xattr,yattr,iattr) in zip(xattrs,yattrs,iattrs):
		key = (xattr,yattr,iattr)
		variables[key] = {}
		for index in range(sizes[key[1]]):
			variables[key][index] = {}
			for i in itertools.product(*[parameters[l] for l in key[2]]):
				params = dict(zip(key[2],i))
				names = [name for name in data if all(data[name][p] == params[p] for p in params)]
				variables[key][index][i] = {}

				xy = {'x':np.array([data[name][key[0]] for name  in names]),'y':np.array([data[name][key[1]] for name  in names])}

				variables[key][index][i]['argsort'] = np.argsort(xy['x'] if xy['x'].ndim < 2 else xy['x'][:,index,0] if key[0] in attrs else np.arange(len(names)))
				variables[key][index][i]['x'] = ((xy['x'] if xy['x'].ndim < 2 else xy['x'][:,index] if key[0] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)
				variables[key][index][i]['y'] = ((xy['y'] if xy['y'].ndim < 1 else xy['y'][:,index] if key[1] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)

				variables[key][index][i]['xfunc'] = (
							{'U': variables[key][index][i]['x'],
							 'N': 1/variables[key][index][i]['x'] if (key[0] =='N' and key[1] == 'gap') else variables[key][index][i]['x'],
							 'partition': variables[key][index][i]['x'],
							}[key[0]]
							)
				variables[key][index][i]['yfunc'] = (
							{'energy': variables[key][index][i]['y'],
							 'order': variables[key][index][i]['y'],
							 'gap': variables[key][index][i]['y'],
							 'entanglement': variables[key][index][i]['y']
							}[key[1]]
							)
				variables[key][index][i]['xfit'] = (
							{'U': variables[key][index][i]['xfunc'],
							 'N': variables[key][index][i]['xfunc'],
							 'partition': variables[key][index][i]['xfunc'],
							}[key[0]]
							)
				variables[key][index][i]['yfit'] = (
							{'energy': variables[key][index][i]['yfunc'],
							 'order': variables[key][index][i]['yfunc'],
							 'gap': variables[key][index][i]['yfunc'],
							 'entanglement': fit(
							 	np.log(params['N']/np.pi*np.sin(np.pi*variables[key][index][i]['xfunc']))/3,
							 	variables[key][index][i]['yfunc'],
							 	intercept=True)[0] if key[1] == 'entanglement' else variables[key][index][i]['y'],
							}[key[1]]
							)
				variables[key][index][i]['xcoef'] = (
							{'U': variables[key][index][i]['xfunc'],
							 'N': variables[key][index][i]['xfunc'],
							 'partition': variables[key][index][i]['xfunc'],
							}[key[0]]
							)
				variables[key][index][i]['ycoef'] = (
							{'energy': variables[key][index][i]['yfunc'],
							 'order': variables[key][index][i]['yfunc'],
							 'gap': variables[key][index][i]['yfunc'],
							 'entanglement': fit(
							 	np.log(params['N']/np.pi*np.sin(np.pi*variables[key][index][i]['xfunc']))/3,
							 	variables[key][index][i]['yfunc'],
							 	intercept=True)[1] if key[1] == 'entanglement' else variables[key][index][i]['y'],
							}[key[1]]
							)


	settings = {
		**{
		key:{
		index: {
			'ax': {
				'plot':[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					'marker':'o',
					'color':getattr(plt.cm,'tab10')(l),
					'label':r'$%s$'%('~'.join(['%s: %s'%(k,texify(j,usetex=False)) for k,j in dict(zip(key[2],i)).items()])),
					}
					for l,i in enumerate(variables[key][index])
					if (dict(zip(key[2],i)).get('U',-1) == -1 and
					    dict(zip(key[2],i)).get('J',-1) == -1)
					],
				'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(key[0])},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xticks':{'ticks':np.linspace(
					np.min([variables[key][index][i]['xfunc'] for i in variables[key][index]]),
					np.max([variables[key][index][i]['xfunc'] for i in variables[key][index]]),
					5)},
				'legend':{'title':r'$%s$'%('~,'.join(['%s'%(texify(k,usetex=False)) for k,j in dict(zip(key[2],i)).items()])),'ncol':1},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2][0]))},
				# 'close':{'fig':True},
			},
			'style':{
				'layout':{
					'nrows':1,
					'ncols':sizes[key[1]],
					'index':index+1,
				},
			},			
		}
		for index in range(sizes[key[1]])}
		for key in [('U','energy',('N',)),('U','order',('N',)),('N','energy',('U','J',))]},
		**{
		key:{
		index: {
			'ax': {
				'plot':[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					'marker':'o',
					'color':getattr(plt.cm,'RdYlBu')(
						((dict(zip(key[2],i))['U']-min([dict(zip(key[2],i))['U'] for i in variables[key][index]]))/
						 max(1e-30,(max([dict(zip(key[2],i))['U'] for i in variables[key][index]]) - min([dict(zip(key[2],i))['U'] for i in variables[key][index]]))))),
					'label':texify(dict(zip(key[2],i))[key[2][0]])if index==(0) else None,
					}
					for l,i in enumerate(variables[key][index])
					],
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(r'1/%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				# 'legend':{'title':texify(key[2][0]),'ncol':4},
				'grid':{'visible':True},
				'set_colorbar':{
					'values':[i for i in np.linspace(
						min([dict(zip(key[2],i))['U'] for i in variables[key][index]]),
						max([dict(zip(key[2],i))['U'] for i in variables[key][index]]),
						4*len(variables[key][index]))],
					'colors':[getattr(plt.cm,'RdYlBu')(i/(4*len(variables[key][index]))) for i in range(4*len(variables[key][index]))],
					'size':'3%',
					'pad':0.1,
					'set_ylabel':{'ylabel':texify(key[2][0])},
					'set_yticks':{'ticks':np.linspace(min([dict(zip(key[2],i))['U'] for i in variables[key][index]]),max([dict(zip(key[2],i))['U'] for i in variables[key][index]]),5)},
					}  if index == 0 else {},			
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2][0]))},
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
		for index in [0]} #range(sizes[key[1]]-1)}
		for key in [('N','gap',('U',))]},	
	**{
		key:{
		index: {
			'ax': {
				'plot':[
					*[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					'marker':{0:'v',1:'o',2:'^'}.get(dict(zip(key[2],i))['U'],'*'),
					'linestyle':'--',
					'alpha':{0:0.3,1:0.6,2:0.9}.get(dict(zip(key[2],i))['U'],1),
					'color':getattr(plt.cm,'tab10')(parameters['N'].index(dict(zip(key[2],i))['N'])),
					'label':'$%s%s : c = %0.3f,~ C = %0.3f$'%(
						'~~'.join(['%s: %s'%(k,texify(j,usetex=False)) for k,j in dict(zip(key[2],i)).items()]),
						r'~' if dict(zip(key[2],i))['N']<10 else '',
						*variables[key][index][i]['ycoef']) if index==(0) else None,					
					}
					for l,i in enumerate(sorted(variables[key][index],key=lambda i: dict(zip(key[2],i))['U']))
					# if dict(zip(key[2],i))['U'] == 1					
					],
					*[{
					'x': variables[key][index][i]['xfit'],
					'y': variables[key][index][i]['yfit'],
					'linestyle':'-',					
					'alpha':{0:0.3,1:0.6,2:0.9}.get(dict(zip(key[2],i))['U'],1),					
					'color':getattr(plt.cm,'tab10')(parameters['N'].index(dict(zip(key[2],i))['N'])),
					}
					for l,i in enumerate(sorted(variables[key][index],key=lambda i: dict(zip(key[2],i))['U']))
					# if dict(zip(key[2],i))['U'] == 1					
					]]
					,
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(r'%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xlim':{'xmin':0,'xmax':1},				
				'set_xticks':{'ticks':[0,0.25,0.5,0.75,1]},				
				'legend':{'title':texify(key[2][0]),'ncol':1,'loc':(1.1,0.35)},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':16,'h':12},
				'subplots_adjust':{},				
				'tight_layout':{},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],key[2][0]))},
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
		for index in [0]} #range(sizes[key[1]]-1)}
		for key in [('partition','entanglement',('N','U',))]},			
	}

	# key = ('partition','entanglement',('N','U',))
	# index = 0
	# i = (parameters['N'][-1],1)
	# print(variables[key][index][i]['yfunc'])

	for k,key in enumerate(settings):
		plot(settings=settings[key])


if __name__ == '__main__':
	main(sys.argv[1:])