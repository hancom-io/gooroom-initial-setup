eulasdir = $(pkgdatadir)/eulas
eulas_DATA = \
	user_agreements_en.txt \
	user_agreements_ko.txt 

noinst_LTLIBRARIES = libgiseulas.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir) \
	-DPKGDATADIR=\"$(pkgdatadir)\"

BUILT_SOURCES = \
	eulas-resources.c \
	eulas-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/eulas.gresource.xml)
eulas-resources.c: eulas.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
eulas-resources.h: eulas.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgiseulas_la_SOURCES = \
	$(BUILT_SOURCES) \
	gis-eulas-page.c \
	gis-eulas-page.h \
	utils.c \
	utils.h

libgiseulas_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(PANGO_CFLAGS)

libgiseulas_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(PANGO_LIBS)

libgiseulas_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

EXTRA_DIST =			\
	eulas.gresource.xml	\
	$(resource_files)	\
	$(NULL)

CLEANFILES = $(BUILT_SOURCES)
