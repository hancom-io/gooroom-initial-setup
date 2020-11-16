noinst_LTLIBRARIES = libgisnetwork.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir)

BUILT_SOURCES = \
	network-resources.c \
	network-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/network.gresource.xml)
network-resources.c: network.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
network-resources.h: network.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgisnetwork_la_SOURCES = \
	$(BUILT_SOURCES) \
	gis-network-page.h \
	gis-network-page.c \
	gis-connection-editor-window.h \
	gis-connection-editor-window.c \
	network-dialogs.h \
	network-dialogs.c

libgisnetwork_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(LIBNM_CFLAGS) \
	$(LIBNMA_CFLAGS)

libgisnetwork_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(LIBNM_LIBS) \
	$(LIBNMA_LIBS)

libgisnetwork_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

EXTRA_DIST = \
	network.gresource.xml \
	$(resource_files)

CLEANFILES = $(BUILT_SOURCES)
