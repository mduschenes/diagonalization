#!/usr/bin/env python

# Import python modules
import sys,os,glob
import datetime,itertools,json,subprocess
from shutil import copy2 as cp
from distutils.dir_util import copy_tree as cpdir
import numpy as np

CALL = 1 

ARGS = {
	'local':{
		'args': lambda args: ['./%s'%(args[0]),*args[1:]],
		},
	'lsf':{
		#'args': lambda args: ['bsub','<<<',*args],
		'args': lambda args: ['bsub','<',*args[:1]],
		},	
	'null': {
		'args': lambda args: [],
		},
	}

SETTINGS = {}


# glob path
def globify(paths,**kwargs):
	return [p for path in paths for p in glob.glob(os.path.expanduser(path),**kwargs)]

# Join paths
def join(*paths,file=None,ext=None):
	isfile = file is not None
	isext = ext is not None and isfile and not file.endswith(ext)
	paths = list(paths)
	if isfile:	
		if isext:
			file = '%s.%s'%(file,ext)
		file_basename = os.path.basename(file)	
		if len(paths)>0:		
			file = file_basename		
		paths.append(file)
	path = os.path.join(*paths)
	if isfile:
		return path,file_basename
	else:
		return path

# Read json data
def read(path,obj):
	with open(path,'r') as fobj:
		try:
			obj.update(json.load(fobj))
		except Exception as e:
			pass
	return

# Write json data
def write(path,obj):
	mkdir(path)
	with open(path,'w') as fobj:
		try:
			json.dump(obj,fobj)
		except Exception as e:
			pass
	return	

# Safely make directory
def mkdir(path):
	if os.path.isfile(path) or len(os.path.splitext(path)[-1])>0:
		directory = os.path.dirname(path)
	else:
		directory = path
	if directory not in [''] and not os.path.exists(directory):
		os.makedirs(directory)
	return


# Copy file
def copy(sources,destination,name=None,recursive=False,exceptions=[]):
	if recursive:
		sources = ['%s%s%s'%(source,'' if source.endswith(os.path.sep)  else os.path.sep,'**')
					for source in sources]
	exceptions = globify(exceptions)

	mkdir(destination)
	if os.path.isdir(destination) and name is not None:
		destination,name = join(destination,file=name)
	for source in globify(sources):
		if source not in exceptions:
			cp(source,destination)
		# except:
		# 	try:
		# 		cpdir(source,destination)
		# 	except:
		# 		pass
	return

# Class to safely change directories
class cd(object):
	def __init__(self,path):
		self.path = path
	def __enter__(self):
		self.cwd = os.getcwd()
		os.chdir(self.path)
	def __exit__(self,etype, value, traceback):
		os.chdir(self.cwd)



# Submit call to command line
def submit(*args,directory='.',call=CALL):
	with cd(directory):
		if call:
			# try:
				args=' '.join(args)
				stdout = os.system(args)
			# except:
			# 	pass
		else:
			print(*args)
	return

# Get all combinations of dictionary of lists
def permute(dictionary,_groups=None,_keep_order=False):

	def indexer(keys,values,_groups):
		if _groups is not None:
			inds = [[keys.index(k) for k in g] for g in _groups]
		else:
			inds = []
			_groups = []
		N = len(_groups)
		_groups.extend([[k] for k in keys if all([k not in g for g in _groups])])
		inds.extend([[keys.index(k) for k in g] for g in _groups[N:]])
		values = [[values[j] for j in i ] for i in inds]
		return _groups,values

	def zipper(keys,values): 
		return [dict(zip(keys,v)) for v in zip(*values)]

	def unzipper(dictionary):
		keys, values = zip(*dictionary.items())	
		return keys,values

	def permuter(dictionaries): 
		return [{k:d[k] for d in dicts for k in d} for dicts in itertools.product(*dictionaries)]

	if dictionary in [None,{}]:
		return [{}]

	keys,values = unzipper(dictionary)
	
	_keys = keys
	
	keys,values = indexer(keys,values,_groups)

	dictionaries = [zipper(k,v) for k,v in zip(keys,values)]

	dictionaries = permuter(dictionaries)

	if _keep_order:
		for i,d in enumerate(dictionaries):
			dictionaries[i] = {k: dictionaries[i][k] for k in _keys}    
	return dictionaries


def main(cwd,src,jobs,paths,args=None,settings={},default=None,**kwargs):
	jobs = globify(jobs)
	paths = globify(paths)
	if len(jobs) == 1:
		jobs = [jobs[0] for path in paths]

	args = [] if args is None else args

	directory = datetime.datetime.today().strftime("%Y_%m_%d-%H_%M_%S")
	iteration = -1

	for job,path in zip(jobs,paths):
		
		_settings = {}
		read(path,_settings)

		folder = join(cwd,directory)
		_file,file = join(folder,file=path,ext='all')
		write(_file,{**settings,
				**{k:[_settings[k]] 
					for k in _settings 
					if k not in settings}})

		for _sets in permute(settings):
			iteration += 1

			_settings.update(_sets)
	
			folder = join(cwd,directory,str(iteration))
			file,_file = join(folder,file=default if default is not None else path)
			job,_job = join(file=job)

			copy([src],folder,recursive=True,exceptions=['*.*'.join([*f.split('.')[:-1],'']) for f in [path,job]])
			copy([job],folder)
			write(file,_settings)

			if callable(args):
				_args = args([_job,_file])
			else:
				_args = args[:]
				_args.extend([_job,_file])

			submit(*_args,directory=folder)

	return





if __name__ == "__main__":
	args = sys.argv[1:]
	if len(args) >= 5:
		arguments = ARGS.get(args[0],ARGS['null'])['args']
		settings = SETTINGS
		cwd = args[1]
		src = args[2]
		default = args[3]
		jobs = args[4:5]
		paths = args[5:]
		main(cwd,src,jobs,paths,arguments,settings,default)
