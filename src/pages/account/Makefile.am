noinst_LTLIBRARIES = libgisaccount.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir)

BUILT_SOURCES = \
	account-resources.c \
	account-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/account.gresource.xml)
account-resources.c: account.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
account-resources.h: account.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgisaccount_la_SOURCES = \
	$(BUILT_SOURCES) \
	gis-account-page.h \
	gis-account-page.c \
	um-utils.h \
	um-utils.c \
	pw-utils.h \
	pw-utils.c

libgisaccount_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(PWQUALITY_CFLAGS)

libgisaccount_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(PWQUALITY_LIBS)

libgisaccount_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

EXTRA_DIST = \
	account.gresource.xml \
	$(resource_files)

CLEANFILES = $(BUILT_SOURCES)
