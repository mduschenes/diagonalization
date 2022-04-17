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
		# 'energy':r'$\epsilon - \epsilon_0 / N$'%(),
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
	try:
		coef = np.linalg.lstsq(x,y)[0] + 0.0
		y = x.dot(coef)
	except:
		y = y
		coef = np.zeros(x.shape[1])
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

	keys = [
		('h','energy',('N',)),
		('h','order',('N',)),
		# ('N','gap',('h',)),
		# ('partition','entanglement',('N','h',)),
		('N','energy',('h','J',)),
		# ('N','entanglement',('h','J')),
	]

	variables = {}

	for key in keys:
		variables[key] = {}
		for index in range(sizes[key[1]]) if key[1] in sizes else []:
			variables[key][index] = {}
			for i in itertools.product(*[parameters[l] for l in key[2]]):
				params = dict(zip(key[2],i))
				names = [name for name in data if all(data[name][p] == params[p] for p in params)]
				unique = {p: [name for name in names if all([data[name][k] == j for k,j in zip(parameters,p)])]
						  for p in itertools.product(*[parameters[k] for k in parameters])
						  if all([params[k] == dict(zip(parameters,p))[k] for k in params]) and len([name for name in names if all([data[name][k] == j for k,j in zip(parameters,p)])]) > 0
						  }
				variables[key][index][i] = {}

				if key == ('N','entanglement',('h','J',)):
					attr = 'partition'
					name = names[0]
					indexes = {name: np.where(data[name][attr][index]==0.5)[0][0] for name in names}
					xy = {
						'x':np.array([np.array([data[name][key[0]] for name in unique[p]]).mean(0) for p in unique]),
						'y':np.array([np.array([data[name][key[1]][:,indexes[name]] for name in unique[p]]).mean(0) for p in unique]),
						'xerr':np.array([np.array([data[name][key[0]] for name in unique[p]]).std(0) for p in unique]),
						'yerr':np.array([np.array([data[name][key[1]][:,indexes[name]] for name in unique[p]]).std(0) for p in unique]),
						}											
				else:
					xy = {
						'x':np.array([np.array([data[name][key[0]] for name in unique[p]]).mean(0) for p in unique]),
						'y':np.array([np.array([data[name][key[1]] for name in unique[p]]).mean(0) for p in unique]),
						'xerr':np.array([np.array([data[name][key[0]] for name in unique[p]]).std(0) for p in unique]),
						'yerr':np.array([np.array([data[name][key[1]] for name in unique[p]]).std(0) for p in unique]),
						}



				variables[key][index][i]['argsort'] = np.argsort(xy['x'] if xy['x'].ndim < 2 else xy['x'][:,index,0] if key[0] in attrs else np.arange(len(names)))
				variables[key][index][i]['x'] = ((xy['x'] if xy['x'].ndim < 2 else xy['x'][:,index] if key[0] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)
				variables[key][index][i]['y'] = ((xy['y'] if xy['y'].ndim < 2 else xy['y'][:,index] if key[1] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)
				variables[key][index][i]['xerr'] = ((xy['xerr'] if xy['xerr'].ndim < 2 else xy['xerr'][:,index] if key[0] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)
				variables[key][index][i]['yerr'] = ((xy['yerr'] if xy['yerr'].ndim < 2 else xy['yerr'][:,index] if key[1] in attrs else np.arange(len(names)))[variables[key][index][i]['argsort']]).reshape(-1)#/variables[key][index][i]['x']


				variables[key][index][i]['xfunc'] = ({
						('h','energy',('N',)): variables[key][index][i]['x'] if (key == ('h','energy',('N',))) else variables[key][index][i]['x'],
						('h','order',('N',)): variables[key][index][i]['x'] if (key == ('h','order',('N',))) else variables[key][index][i]['x'],
						('N','gap',('h',)): 1/variables[key][index][i]['x'] if (key == ('N','gap',('h',))) else variables[key][index][i]['x'],
						('partition','entanglement',('N','h',)): variables[key][index][i]['x'] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['x'],
						('N','energy',('h','J',)): variables[key][index][i]['x'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['x'],
						('N','entanglement',('h','J',)): variables[key][index][i]['x'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['x'],
						}[key])

				variables[key][index][i]['yfunc'] = ({
						('h','energy',('N',)): variables[key][index][i]['y'] if (key == ('h','energy',('N',))) else variables[key][index][i]['y'],
						('h','order',('N',)): variables[key][index][i]['y'] if (key == ('h','order',('N',))) else variables[key][index][i]['y'],
						('N','gap',('h',)): 1/variables[key][index][i]['y'] if (key == ('N','gap',('h',))) else variables[key][index][i]['y'],
						('partition','entanglement',('N','h',)): variables[key][index][i]['y'] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['y'],
						('N','energy',('h','J',)): variables[key][index][i]['y'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['y'],
						('N','entanglement',('h','J',)): variables[key][index][i]['y'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['y'],
						}[key])				

				variables[key][index][i]['xfit'] = ({
						('h','energy',('N',)): variables[key][index][i]['xfunc'] if (key == ('h','energy',('N',))) else variables[key][index][i]['xfunc'],
						('h','order',('N',)): variables[key][index][i]['xfunc'] if (key == ('h','order',('N',))) else variables[key][index][i]['xfunc'],
						('N','gap',('h',)): 1/variables[key][index][i]['xfunc'] if (key == ('N','gap',('h',))) else variables[key][index][i]['xfunc'],
						('partition','entanglement',('N','h',)): variables[key][index][i]['xfunc'] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['xfunc'],
						('N','energy',('h','J',)): variables[key][index][i]['xfunc'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['xfunc'],
						('N','entanglement',('h','J',)): variables[key][index][i]['xfunc'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['xfunc'],
						}[key])

				variables[key][index][i]['yfit'] = ({
						('h','energy',('N',)): variables[key][index][i]['yfunc'] if (key == ('h','energy',('N',))) else variables[key][index][i]['yfunc'],
						('h','order',('N',)): variables[key][index][i]['yfunc'] if (key == ('h','order',('N',))) else variables[key][index][i]['yfunc'],
						('N','gap',('h',)): 1/variables[key][index][i]['yfunc'] if (key == ('N','gap',('h',))) else variables[key][index][i]['yfunc'],
						('partition','entanglement',('N','h',)): fit(
							 	np.log(params['N']/np.pi*np.sin(np.pi*variables[key][index][i]['xfunc']))/3,
							 	variables[key][index][i]['yfunc'],
							 	intercept=True)[0] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['yfunc'],
						('N','energy',('h','J',)): variables[key][index][i]['yfunc'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['yfunc'],
						('N','entanglement',('h','J',)): variables[key][index][i]['yfunc'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['yfunc'],
						}[key])
	
				variables[key][index][i]['xcoef'] = ({
						('h','energy',('N',)): variables[key][index][i]['xfunc'] if (key == ('h','energy',('N',))) else variables[key][index][i]['xfunc'],
						('h','order',('N',)): variables[key][index][i]['xfunc'] if (key == ('h','order',('N',))) else variables[key][index][i]['xfunc'],
						('N','gap',('h',)): 1/variables[key][index][i]['xfunc'] if (key == ('N','gap',('h',))) else variables[key][index][i]['xfunc'],
						('partition','entanglement',('N','h',)): variables[key][index][i]['xfunc'] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['xfunc'],
						('N','energy',('h','J',)): variables[key][index][i]['xfunc'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['xfunc'],
						('N','entanglement',('h','J',)): variables[key][index][i]['xfunc'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['xfunc'],
						}[key])

				variables[key][index][i]['ycoef'] = ({
						('h','energy',('N',)): variables[key][index][i]['yfunc'] if (key == ('h','energy',('N',))) else variables[key][index][i]['yfunc'],
						('h','order',('N',)): variables[key][index][i]['yfunc'] if (key == ('h','order',('N',))) else variables[key][index][i]['yfunc'],
						('N','gap',('h',)): 1/variables[key][index][i]['yfunc'] if (key == ('N','gap',('h',))) else variables[key][index][i]['yfunc'],
						('partition','entanglement',('N','h',)): fit(
							 	np.log(params['N']/np.pi*np.sin(np.pi*variables[key][index][i]['xfunc']))/3,
							 	variables[key][index][i]['yfunc'],
							 	intercept=True)[1] if (key == ('partition','entanglement',('N','h',))) else variables[key][index][i]['yfunc'],
						('N','energy',('h','J',)): variables[key][index][i]['yfunc'] if (key == ('N','energy',('h','J',))) else variables[key][index][i]['yfunc'],
						('N','entanglement',('h','J',)): variables[key][index][i]['yfunc'] if (key == ('N','entanglement',('h','J',))) else variables[key][index][i]['yfunc'],
						}[key])



	settings = {
		**{
		key:{
		index: {
			'ax': {
				'plot':[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					'marker':'o',					
					# 'marker':{0:'',1:'o',2:'^'}.get(index,'o'),					
					# 'color':getattr(plt.cm,'tab10')(l),
					'color':getattr(plt.cm,'viridis')((len(variables[key][index]) - 1 - l)/len(variables[key][index])),
					'label':texify(dict(zip(key[2],i))[key[2][0]])if index==(0) else None,
					}
					for l,i in enumerate(variables[key][index])
					# if ((dict(zip(key[2],i)).get('h',-1) > 0.9 and dict(zip(key[2],i)).get('h',-1) < 1.1) and
					#     (dict(zip(key[2],i)).get('N',1000) >= 8))
					],
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(key[0])},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xticks':{'ticks':np.linspace(
					np.min([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']]),
					np.max([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']]),
					3)},
				'legend':{'title':texify(key[2][0]),'ncol':1,'set_linewidth':{'w':3}},
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
					# 'ncols':1,
					# 'index':1,
				},
			},			
		}
		for index in range(sizes[key[1]])}
		for key in [('h','energy',('N',)),('h','order',('N',))]
		if key in keys
		},
		**{
		key:{
		index: {
			'ax': {
				'plot':[{
					'x': variables[key][index][i]['xfunc'],
					'y': (variables[key][index][i]['yfunc']-variables[key][0][i]['yfunc'])*variables[key][index][i]['xfunc'],
					'marker':'o',					
					# 'marker':{0:'',1:'o',2:'^'}.get(index,'o'),					
					# 'color':getattr(plt.cm,'tab10')(l),
					'color':getattr(plt.cm,'viridis')((len(variables[key][index]) - 1 - l)/len(variables[key][index])),
					'label':texify(dict(zip(key[2],i))[key[2][0]])if index==(0) else None,
					}
					for l,i in enumerate(variables[key][index])
					if ((dict(zip(key[2],i)).get('h',-1) > 0.9 and dict(zip(key[2],i)).get('h',-1) < 1.1) and
					    1 #(dict(zip(key[2],i)).get('N',1000) >= 8)
					    )
					],
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(key[0])},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xticks':{'ticks':np.linspace(
					np.min([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']]),
					np.max([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']]),
					3)},
				# 'legend':{'title':texify(key[2][0]),'ncol':1,'set_linewidth':{'w':3}},
				'grid':{'visible':True},
				'set_colorbar':{
					'values':[i for i in np.linspace(
						min([dict(zip(key[2],i))['h'] for i in variables[key][index]]),
						max([dict(zip(key[2],i))['h'] for i in variables[key][index]]),
						4*len(variables[key][index]))],
					'colors':[getattr(plt.cm,'viridis')(i/(4*len(variables[key][index]))) for i in range(4*len(variables[key][index]))],
					'size':'3%',
					'pad':0.1,
					'set_ylabel':{'ylabel':texify(key[2][0])},
					'set_yticks':{'ticks':np.linspace(min([dict(zip(key[2],i))['h'] for i in variables[key][index]]),max([dict(zip(key[2],i))['h'] for i in variables[key][index]]),5)},
					# }  if index == (sizes[key[1]]-1) else {},							
					}  if index == (1) else {},							
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
					'ncols':1,
					'index':1,
				},
			},			
		}
		for index in [1]} #range(1,sizes[key[1]])}
		for key in [('N','energy',('h','J',))]
		if key in keys
		},		
		**{
		key:{
		index: {
			'ax': {
				'errorbar':[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc']/variables[key][index][i]['xfunc'],
					**({
					# 'xerr': variables[key][index][i]['xerr'],
					'yerr': variables[key][index][i]['yerr']/variables[key][index][i]['xfunc'],
					'capsize':5,
					'elinewidth':2,
					} if variables[key][index][i]['yerr'].sum()>0 else {}),
					'marker':'o',
					'color':getattr(plt.cm,'tab10')(l),
					'label':r'$%s$'%('~'.join(['%s: %s'%(texify(k,usetex=False),texify(j,usetex=False)) for k,j in dict(zip(key[2],i)).items()])),
					}
					for l,i in enumerate(variables[key][index])
					# if (dict(zip(key[2],i)).get('h',0.0) == 0.0)
					# if (dict(zip(key[2],i)).get('h',0.5) == 0.5)
					# if (dict(zip(key[2],i)).get('h',-1) == -1 and
					#     dict(zip(key[2],i)).get('J',-1) == -1)
					],
				'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(key[0])},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_yscale':{'value':'log'},				
				'set_xticks':{'ticks':np.arange(
					np.min([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']]),
					np.max([u for i in variables[key][index] for u in variables[key][index][i]['xfunc']])+2,
					2)
					},
				'legend':{
					'title':r'$%s$'%(',~'.join(['%s'%(texify(k,usetex=False)) for k,j in dict(zip(key[2],i)).items()])),
					'ncol':1,
					'set_errorbar':False},
				'grid':{'visible':True},
			},	
			'fig':{
				'set_size_inches':{'w':18,'h':12},
				'savefig':{'fname':os.path.join(directory,'%s__%s__%s.pdf'%(key[1],key[0],'__'.join(key[2])))},
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
		for key in [('N','entanglement',('h','J'))]
		if key in keys
		},		
		**{
		key:{
		index: {
			'ax': {
				'plot':[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					'marker':'o',
					'color':getattr(plt.cm,'RdYlBu')(
						((dict(zip(key[2],i))['h']-min([dict(zip(key[2],i))['h'] for i in variables[key][index]]))/
						 max(1e-30,(max([dict(zip(key[2],i))['h'] for i in variables[key][index]]) - min([dict(zip(key[2],i))['h'] for i in variables[key][index]]))))),
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
						min([dict(zip(key[2],i))['h'] for i in variables[key][index]]),
						max([dict(zip(key[2],i))['h'] for i in variables[key][index]]),
						4*len(variables[key][index]))],
					'colors':[getattr(plt.cm,'RdYlBu')(i/(4*len(variables[key][index]))) for i in range(4*len(variables[key][index]))],
					'size':'3%',
					'pad':0.1,
					'set_ylabel':{'ylabel':texify(key[2][0])},
					'set_yticks':{'ticks':np.linspace(min([dict(zip(key[2],i))['h'] for i in variables[key][index]]),max([dict(zip(key[2],i))['h'] for i in variables[key][index]]),5)},
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
		for key in [('N','gap',('h',))]
		if key in keys
		},	
	**{
		key:{
		index: {
			'ax': {
				'errorbar':[
					*[{
					'x': variables[key][index][i]['xfunc'],
					'y': variables[key][index][i]['yfunc'],
					**({
					# 'xerr': variables[key][index][i]['xerr'],
					'yerr': variables[key][index][i]['yerr'],
					} if variables[key][index][i]['yerr'].sum()>0 else {}),					
					'marker':{0:'v',1:'o',2:'^'}.get(dict(zip(key[2],i))['h'],'*'),
					'linestyle':'--',
					'alpha':{0:0.3,1:0.6,2:0.9}.get(dict(zip(key[2],i))['h'],1),
					'color':getattr(plt.cm,'tab10')(parameters['N'].index(dict(zip(key[2],i))['N'])),
					'label':'$%s%s : c = %0.3f,~ C = %0.3f$'%(
						'~~'.join(['%s: %s'%(k,texify(j,usetex=False)) for k,j in dict(zip(key[2],i)).items()]),
						r'~' if dict(zip(key[2],i))['N']<10 else '',
						*variables[key][index][i]['ycoef']) if index==(0) else None,					
					}
					for l,i in enumerate(sorted(variables[key][index],key=lambda i: dict(zip(key[2],i))['h']))
					if dict(zip(key[2],i))['h'] == 1					
					],
					*[{
					'x': variables[key][index][i]['xfit'],
					'y': variables[key][index][i]['yfit'],
					'linestyle':'-',					
					'alpha':{0:0.3,1:0.6,2:0.9}.get(dict(zip(key[2],i))['h'],1),					
					'color':getattr(plt.cm,'tab10')(parameters['N'].index(dict(zip(key[2],i))['N'])),
					}
					for l,i in enumerate(sorted(variables[key][index],key=lambda i: dict(zip(key[2],i))['h']))
					if dict(zip(key[2],i))['h'] == 1					
					]]
					,
				# 'set_title':{'label':r'$\textrm{Level %d}$'%(index)},
				'set_xlabel':{'xlabel':texify(r'%s'%(key[0]))},
				'set_ylabel':{'ylabel':texify(key[1])},
				'set_xscale':{'value':'linear'},				
				'set_xlim':{'xmin':0,'xmax':1},				
				'set_xticks':{'ticks':[0,0.25,0.5,0.75,1]},				
				'legend':{'title':texify(key[2][0]),'ncol':1,'loc':(1.1,0.35),'set_errorbar':False},
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
		for key in [('partition','entanglement',('N','h',))]
		if key in keys
		},			
	}

	# key = ('partition','entanglement',('N','h',))
	# index = 0
	# i = (parameters['N'][-1],1)
	# print(variables[key][index][i]['yfunc'])

	for k,key in enumerate(settings):
		plot(settings=settings[key])


if __name__ == '__main__':
	main(sys.argv[1:])