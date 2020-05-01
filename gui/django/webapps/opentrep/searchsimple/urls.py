from django.urls import re_path
from searchsimple import views

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

app_name = 'searchsimple'

urlpatterns = [
    # Examples:
    # url(r'^$', 'opentrep.views.home', name='home'),
    # url(r'^opentrep/', include('opentrep.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),

    #
    re_path(r'^$', views.searchsimple),
    re_path(r'^(?P<query_string>[^/]+)/', views.searchsimple),
]


