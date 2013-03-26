from django.db import models

# Class corresponding to the geoname database table
class POR (models.Model):
    geonameid = models.IntegerField (db_column='geonameid', primary_key=True)
    name = models.CharField (max_length=200, null=True, blank=True)
    asciiname = models.CharField (max_length=200, null=True, blank=True)
    alternatenames = models.CharField (max_length=4000, null=True, blank=True)
    latitude = models.DecimalField (max_digits=10, decimal_places=7, null=True,
                                    blank=True)
    longitude = models.DecimalField (max_digits=10, decimal_places=7, null=True,
                                     blank=True)
    fclass = models.CharField (max_length=1, null=True, blank=True)
    fcode = models.CharField (max_length=10, null=True, blank=True)
    country = models.CharField (max_length=2, null=True, blank=True)
    cc2 = models.CharField (max_length=60, null=True, blank=True)
    admin1 = models.CharField (max_length=20, null=True, blank=True)
    admin2 = models.CharField (max_length=80, null=True, blank=True)
    admin3 = models.CharField (max_length=20, null=True, blank=True)
    admin4 = models.CharField (max_length=20, null=True, blank=True)
    population = models.IntegerField (null=True, blank=True)
    elevation = models.IntegerField (null=True, blank=True)
    gtopo30 = models.IntegerField (null=True, blank=True)
    timezone = models.CharField (max_length=40, null=True, blank=True)
    moddate = models.DateField ('modification date', null=True, blank=True)
    def __unicode__(self):
        return '%s, %s (%d, %d)' % (self.name, self.country, 
                                    self.latitude, self.longitude)

    class Meta:
        db_table = 'geoname'

# Class corresponding to the alternate_name database table, linked to the
# geoname table
class AlternateName (models.Model):
    alternatenameid = models.IntegerField (db_column='alternatenameid',
                                           primary_key=True)
    geonameid = models.ForeignKey (POR, db_column='geonameid', null=True)
    isoLanguage = models.CharField (max_length=7, null=True, blank=True)
    alternateName = models.CharField (max_length=200, null=True, blank=True)
    isPreferredName = models.PositiveSmallIntegerField (null=True, blank=True)
    isShortName = models.PositiveSmallIntegerField (null=True, blank=True)
    def __unicode__(self):
        # The POR name can be accessed thanks to self.geonameid.name
        return '%s, %s' % (self.isoLanguage, self.alternateName)

    class Meta:
        db_table = 'alternate_name'
