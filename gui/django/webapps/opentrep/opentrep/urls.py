from django.conf.urls import patterns, include, url
#from search import views

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

handler404 = 'search.views.custom_404_view'
handler500 = 'search.views.custom_500_view'

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'opentrep.views.home', name='home'),
    # url(r'^opentrep/', include('opentrep.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),

    # Catch-all
    url(r'^$', include('search.urls', namespace="root")),
    # url(r'^$', views.index, name='home'),

    # The following two directives do not match if the catch-all directive
    # above is active (not commented)
    url(r'^searchsimple/', include('searchsimple.urls',
                                   namespace="searchsimple")),
    url(r'^search/', include('search.urls', namespace="search")),
)
