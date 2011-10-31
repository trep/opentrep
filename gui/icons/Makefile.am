# gui/icons sub-directory: Images (.png, .gif, etc)
include $(srcdir)/sources.mk

datadir = @datadir@
pkgdatadir = $(datadir)/@PACKAGE@
imgdir = $(pkgdatadir)/gui/icons

MAINTAINERCLEANFILES = Makefile.in Makefile

noinst_DATA =  $(img_sources)

EXTRA_DIST = $(noinst_DATA)

# Targets
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(imgdir); \
	for f in $(noinst_DATA); do \
		$(INSTALL_DATA) $$f $(DESTDIR)$(imgdir); \
	done

uninstall-local:
	rm -rf $(DESTDIR)$(imgdir)

clean-local:
	rm -rf *.log *.tag
