#!/usr/bin/env python

# Import python modules
import pytest
import os,sys,itertools
import numpy as np
import scipy as sp
import scipy.sparse

import jax
import jax.numpy as jnp
import jax.scipy as jsp
from jax import jit
jax.config.update('jax_platform_name','cpu')
import qiskit as qs	


# Import user modules
ROOT = os.path.dirname(os.path.abspath(__file__))
PATHS = ['',"..","../..","../../lib"]
for PATH in PATHS:
	sys.path.append(os.path.abspath(os.path.join(ROOT,PATH)))



