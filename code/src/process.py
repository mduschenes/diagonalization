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

	file = h5py.File(path,'r')
	attributes = ['eigenvalues','eigenvectors']

	data = {}

	for group in file:
		data[group] = {}

		names = list(set((name.replace('.real','').replace('.imag','') for name in file[group])))
		for name in names:

			try:
				name_real = "%s.%s"%(name,'real')
				data_real = file[group][name_real][...]

				name_imag = "%s.%s"%(name,'imag')
				data_imag = file[group][name_imag][...]

				data[group][name] = data_real + 1j*data_imag
			except:
				data[group][name] = file[group][name][...]

		names = list(set((name for name in file[group].attrs)))
		for name in names:
			data[group][name] = file[group].attrs[name]

	# print(data.shape,data.dtype)
	print(data)

	# data = load(params['data'])

	# plot(data)



if __name__ == '__main__':
	main(sys.argv[1:])