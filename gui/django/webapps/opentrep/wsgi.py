"""
WSGI config for opentrep project.

It exposes the WSGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/2.2/howto/deployment/wsgi/
"""

import sys

path = '/var/www/webapps/search'
if path not in sys.path:
    sys.path.append(path)

import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'opentrep.settings')

application = get_wsgi_application()


