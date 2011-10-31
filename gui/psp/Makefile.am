# Python Server Pages (PSP)
include $(srcdir)/sources.mk

datadir = @datadir@
pkgdatadir = $(datadir)/@PACKAGE@
pspdir = $(pkgdatadir)/gui/psp

MAINTAINERCLEANFILES = Makefile.in Makefile

noinst_DATA =  $(html_sources) $(py_sources) $(psp_sources)

EXTRA_DIST = $(noinst_DATA)

# Targets
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pspdir); \
	for f in $(noinst_DATA); do \
		$(INSTALL_DATA) $$f $(DESTDIR)$(pspdir); \
	done

uninstall-local:
	rm -rf $(DESTDIR)$(pspdir)

clean-local:
	rm -rf *.log *.tag
