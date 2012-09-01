from django.conf.urls.defaults import *
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Map ^trep to api/urls.py (api.urls module)
    (r'^trep/api/', include ('api.urls')),

    # Map ^test to test/views.py (test module)
    (r'^trep/test/', 'test.views.myview'),

    # Uncomment the admin/doc line below to enable admin documentation:
    (r'^trep/admin/doc/', include ('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^trep/admin/', include (admin.site.urls)),
)

