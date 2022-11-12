#!/usr/bin/env python
import pytest

def test_mkl():
	
	import numpy as np
	print(np.__file__)
	np.show_config()

	import tenpy as tp
	print(tp.__file__)
	tp.show_config()

	return

