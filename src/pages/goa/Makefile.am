noinst_LTLIBRARIES = libgisgoa.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir)

BUILT_SOURCES = \
	goa-resources.c \
	goa-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/goa.gresource.xml)
goa-resources.c: goa.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
goa-resources.h: goa.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgisgoa_la_SOURCES =	\
	$(BUILT_SOURCES) \
	gis-goa-page.h	\
	gis-goa-page.c

libgisgoa_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GOA_CFLAGS) \
	$(GOA_BACKEND_CFLAGS) \
	$(LIBSECRET_CFLAGS)

libgisgoa_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(GOA_LIBS) \
	$(GOA_BACKEND_LIBS) \
	$(LIBSECRET_LIBS)

libgisgoa_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

EXTRA_DIST =			\
	goa.gresource.xml	\
	$(resource_files)	\
	$(NULL)

CLEANFILES = $(BUILT_SOURCES)
