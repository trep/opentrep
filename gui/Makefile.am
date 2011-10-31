# Python Server Pages (PSP)

SUBDIRS = icons psp

MAINTAINERCLEANFILES = Makefile.in Makefile

datadir = @datadir@
pkgdatadir = $(datadir)/@PACKAGE@
guidir = $(pkgdatadir)/gui

psp_sources = psp
psp_dests = $(foreach ext,.tar.gz .tar.bz2,$(addsuffix $(ext),$(psp_sources)))

# Targets
$(top_builddir)/%.tar.gz $(builddir)/%.tar.gz: %/*.html %/*.py %/*.psp %/../icons/*.png
	tar chof - $^ | gzip --best -c > $@

$(top_builddir)/%.tar.bz2 $(builddir)/%.tar.bz2: %/*.html %/*.py %/*.psp %/../icons/*.png
	tar chof - $^ | bzip2 -9 -c > $@

dist-gui: $(addprefix $(top_builddir)/,$(psp_dests))

clean-local:
	rm -f $(addprefix $(top_builddir)/,$(psp_dests))

snapshot-gui:
	$(MAKE) dist-gui gui_tarname=@PACKAGE_TARNAME@-gui-`date +"%Y%m%d"`
