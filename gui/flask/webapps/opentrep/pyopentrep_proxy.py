#!/usr/bin/python

import sys

def import_pyopentrep(pyopentrep_path):
	sys.path.append(pyopentrep_path)
	import pyopentrep
	return pyopentrep
