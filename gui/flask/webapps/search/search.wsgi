"""
WSGI config for opentrep project.

Attempt to configure a Flask application.
See http://flask.pocoo.org/docs/0.12/deploying/mod_wsgi/ for more details.

"""
import sys

path = '/var/www/webapps/'
if path not in sys.path:
    sys.path.append(path)

import os

# Apply WSGI middleware here.
from search import app as application

