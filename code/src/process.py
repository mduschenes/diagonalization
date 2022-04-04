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
from lib.utils.plot import plot


def main(args):

	path = args[0]
	wr = 'r'
	default = {}
	kwargs = {}

	data = load(path,wr=wr,default=default,**kwargs)

	print(data)


if __name__ == '__main__':
	main(sys.argv[1:])