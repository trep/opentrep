from django.db import models

# Create your models here.

class Details (models.Model):
	code = models.CharField (max_length=3)
	longitude = models.FloatField()
	latitude = models.FloatField()

class Names (models.Model):
	place = models.ForeignKey (Details)
	language_code = models.CharField (max_length=2)
	classical_name = models.CharField (max_length=30)

