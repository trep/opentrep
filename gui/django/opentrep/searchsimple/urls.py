from django.conf.urls import patterns, include, url
from searchsimple import views

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'opentrep.views.home', name='home'),
    # url(r'^opentrep/', include('opentrep.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),

    #
    url(r'^/$', 'search.views.search'),
    url(r'^/(?P<query_string>[^/]+)', 'searchsimple.views.search'),
)
