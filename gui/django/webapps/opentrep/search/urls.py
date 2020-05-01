from django.urls import re_path
from search import views

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

app_name = 'search'

urlpatterns = [
    # Examples:
    # path(r'^$', 'opentrep.views.home', name='home'),
    # path(r'^opentrep/', include('opentrep.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # path(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # path(r'^admin/', include(admin.site.urls)),

    #
    re_path(r'^$', views.index, name='index'),
]

