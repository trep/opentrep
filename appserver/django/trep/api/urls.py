from django.conf.urls.defaults import *
from piston.resource import Resource
from api.handlers import TrepHandler

class CsrfExemptResource (Resource):
	"""Work around for the Cross-site request forgery (CSRF) issue

	See http://en.wikipedia.org/wiki/Cross-site_request_forgery.
	It is surely a bad idea to de-activate it for all the methods.
	See http://docs.djangoproject.com/en/dev/ref/contrib/csrf/
	"""
	def __init__ (self, handler, authentication = None):
		super (CsrfExemptResource, self).__init__ (handler, authentication)
		self.csrf_exempt = getattr (self.handler, 'csrf_exempt', True)

# 
display_resource = CsrfExemptResource (TrepHandler)

#
urlpatterns = patterns ('',
	(r'^(?:q=(?P<queryURL>.+))?$', display_resource),
	(r'^(?P<queryURL>.+)$', display_resource),
)

