import pytest
import os

# Helper functions


# Parse delimeter separated CLI argument as list
def _strparser(value,delimeter=','):
	value = [[float(x) if x not in ['None'] else None for x in v.split(delimeter)] if isinstance(v,str) else v for v in value]
	value = [[int(x) if (isinstance(x,(float,int,str)) and int(x)==x) else x for x in v] for v in value]
	return value

# Check if iterable
def _isiterable(obj):
	field = '__iter__'
	try:
		return hasattr(obj,field)
	except:
		return False


# Get arg values
def _argparser(arg,value,args):
		value = [value] if args[arg]['action'] in ['store','store_true'] else value
		value = args[arg].get('parser',lambda value:value)(value)
		value = value[len(args[arg]['default']):] if (args[arg]['action'] in ['append']) and (len(value)>len(args[arg]['default'])) else value
		value = [v for i,v in enumerate(value) if v not in value[:i] or v == args[arg]['default']]
		return value

# Command line arguments for fixtures with {arg: {action,default,type,allowed,...})}
args = {
	'name':{
		'default':'test',
		'type':str,
		'action':'store',
		'allowed':['test','tmp','data','root'],
		'id':'name',		
		},
	'n':{
		'default':[5],
		'type':int,
		'action':'append',
		'allowed':None,
		'id':'n',
		},
	'p':{
		'default':[2],
		'type':int,
		'action':'append',
		'allowed':[1,2,3,4],
		'id':'p',	
		},
	'chunk':{
		'default':[[None,None]],
		'type':str,
		'action':'append',
		'allowed':None,
		'parser': _strparser,
		'id':'chunk',
		},				
	'n_jobs':{
		'default':[1],
		'type':int,
		'action':'append',
		'allowed':range(8),
		'id':'n_jobs',
		},
	'verb':{
		'default':False,
		'type':bool,
		'action':'store_true',
		'allowed':[True,False],
		'id':'verbose',
		},						
	}


def pytest_addoption(parser):
	for arg in args:
		parser.addoption('--%s'%(arg), **{field: args[arg][field] for field in [*['action','default'],*(['type'] if args[arg]['action'] not in ['store_true'] else [])]})
	return

def pytest_generate_tests(metafunc):
	# This is called for every test. Only get/set command line arguments
	# if the argument is specified in the list of test 'fixturenames'.
	for arg in args:
		value = _argparser(arg,getattr(metafunc.config.option,arg),args)
		allowed = args[arg].get('allowed')
		assert (allowed is None) or (value in allowed) or (_isiterable(value) and all([v in allowed for v in value])), 'CLI argument \'%s\': \'%r\' not in allowed values %r'%(arg,value,allowed)
		if arg in metafunc.fixturenames and value is not None:
			metafunc.parametrize(arg, value)
	return


def pytest_make_parametrize_id(config,val,argname):
	id = ' %s: %r '%(argname,val)
	return id



@pytest.fixture
def path(name):
	if name in ['test']:
		path = os.path.join('test','test.csv')
	elif name in ['tmp']:
		path = os.path.join('tmp','tmp.csv')		
	elif name in ['data']:
		path = os.path.join('data','data.csv')
	elif name in ['root']:
		path = 'data.csv'
	return path


# @pytest.mark.parametrize('n',)
@pytest.fixture
def parameters(n,p,chunk,n_jobs,name,verb):
	if name in ['test']:
		parameters = {'n':n,'p':p,'d':1,'k':2,'r':3,'unique':True,'chunk':chunk,'n_jobs':n_jobs,'verbose':verb}
	elif name in ['tmp']:
		parameters = {'n':n,'p':p,'d':1,'k':2,'r':3,'unique':True,'chunk':chunk,'n_jobs':n_jobs,'verbose':verb}		
	elif name in ['data']:
		parameters = {'n':1351,'p':p,'d':1,'k':2,'r':3,'unique':True,'chunk':chunk,'n_jobs':n_jobs,'verbose':verb}
	elif name in ['root']:
		parameters = {'n':n,'p':p,'d':1,'k':2,'r':3,'unique':True,'chunk':chunk,'n_jobs':n_jobs,'verbose':verb}		
	return parameters
