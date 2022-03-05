#!/usr/bin/env python

# Import python modules
import os,sys,h5py
import numpy as np
import scipy as sp

import jax
import jax.numpy as jnp
import jax.scipy as jsp

# Logging
import logging
conf = "logging.conf"
try: 
	logging.config.fileConfig(conf,disable_existing_loggers=False) 
except: 
	pass
logger = logging.getLogger(__name__)


# Import user modules
ROOT = os.path.dirname(os.path.abspath(__file__))
PATHS = ['',"..","../..","../../lib"]
for PATH in PATHS:
	sys.path.append(os.path.abspath(os.path.join(ROOT,PATH)))

from lib.utils.io import setup,load,dump
from lib.utils.utils import dictionary,objs,array,asarray,ones,zeros,arange,eye,identity,hadamard,phasehadamard,cnot,toffoli
from lib.utils.utils import jit,grad,finitegrad,norm,expm,tensordot,tensorprod,multi_matmul,inner,abs,repeat,allclose,isclose,PRNG,gradinner,prod
from lib.utils.utils import trotter,trottergrad,cos,sin
from lib.utils.parallel import Pooler,Parallelize
from lib.utils.plot import plot



def main(args):

	defaults = [
			{
				"io":{
					"verbose":"Info",
					"path":""
				},
				"logging":{
					"version": 1,
					"disable_existing_loggers":0,
					"formatters": {"file": {"format": "%(asctime)s :: %(message)s","datefmt":"%Y-%m-%d %H:%M:%S"},
								   "stdout": {"format": "%(message)s","datefmt":"%Y-%m-%d %H:%M:%S"}},
					"handlers": {"file": {"class": "logging.FileHandler",
											 "filename": "log.log",
											 "formatter": "file",
											 "level": "INFO",
											 "mode": "w"},
								"stdout": {"class": "logging.StreamHandler",
											  "formatter": "stdout",
											  "level": "INFO",
											  "stream": "ext://sys.stdout"}},
					"root": {"handlers": ["stdout", "file"], "level": "DEBUG"}
				},
				"plotting":{
					"names":["energy","order"],
					"path":"figures",
					"verbose":"info"
				},
				"data":"data/test.csv",
				"model":{
					"N":3,
					"D":2,
					"d":1,
					"lattice":"square",
					"system":{
						"dtype":"complex",
						"format":"array",
						"device":"cpu",
						"verbose":"info"		
					},								
				},
				"parameters":{
					"J": {"start":-2.0,"stop":2.0,"length":5,"type":"range"},
					"g": [1]
				}
			}
			]

	# params = setup(args,defaults)

	path = args[0]

	print(path)

	data = h5py.File(path,'r').get('data')[...]

	# data = load(data,header='infer')
	print(data)

	# data = load(params['data'])

	# plot(data)



if __name__ == '__main__':
	main(sys.argv[1:])