"""opentrep URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
#from django.contrib import admin
from django.urls import path, include
from search import views

app_name = "opentrep"

#handler404 = 'search.views.custom_404_view'
#handler500 = 'search.views.custom_500_view'

urlpatterns = [
    # Catch-all
    path('', include('search.urls')),
    # url(r'^$', views.index, name='home'),

    # The following two directives do not match if the catch-all directive
    # above is active (not commented)
    path('searchsimple/', include('searchsimple.urls')),
    path('search/', include('search.urls')),
]

