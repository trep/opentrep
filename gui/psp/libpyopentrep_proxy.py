#!/usr/bin/python

import sys

def import_libpyopentrep(libpyopentrep_path):
	sys.path.append(libpyopentrep_path)
	import libpyopentrep
	return libpyopentrep
