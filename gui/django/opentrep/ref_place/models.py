from django.db import models

# Create your models here.

class Details (models.Model):
	code = models.CharField (max_length=3, primary_key=True)
	longitude = models.FloatField()
	latitude = models.FloatField()
	def __unicode__(self):
		return '%s (%d, %d)' % (self.code, self.latitude, self.longitude)

class Names (models.Model):
	code = models.ForeignKey (Details, db_column='code', primary_key=True)
	language_code = models.CharField (max_length=2, primary_key=True)
	classical_name = models.CharField (max_length=30)
	def __unicode__(self):
		return '%s (%s, %s)' % (self.code, self.language_code, self.classical_name)

