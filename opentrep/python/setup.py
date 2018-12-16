import ez_setup
ez_setup.use_setuptools()
from setuptools import setup, find_packages
import sys

setup(
    name = "django-opentrep",
    version = '0.6.1',
    description = 'The Django-based Search for Travel Web application',
    packages = find_packages(),
    author = 'Denis.Arnaud',
    author_email = 'denis.arnaud_fedora@m4x.org',
    license = 'LGPL v2.1',
    keywords = 'opentrep, travel, intelligence, search, database, django',
    url = 'http://github.com/trep/opentrep',
    include_package_data = True,
    classifiers = [
        'Development Status :: 4 - Beta',
        'Environment :: Web Environment',
        'Framework :: Django',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: LGPL License',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 2.4',
        'Programming Language :: Python :: 2.5',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: JavaScript',
        'Topic :: Internet :: WWW/HTTP :: WSGI :: Application',
    ],
    long_description = """The ``opentrep`` django app allows to easily
search for any travel-related POR (point of reference).

Setup
========

To the INSTALLED_APPS in your ``settings.py`` add entry ``'opentrep'``.

Usage
============

Examples below show how to use ``opentrep``

    import Travel_pb2
    import pyopentrep
    openTrepLibrary = pyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init ('/tmp/opentrep/xapian_traveldb', 'nodb', '', 0, 'pyopentrep.log')
    if initOK == False:
        errorMsg = 'Error: The OpenTrepLibrary cannot be initialised'
        raise Exception (errorMsg)

    result = openTrepLibrary.search ('J', 'nce sfo')
    print str(result)

"""
)
