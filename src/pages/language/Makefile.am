noinst_LTLIBRARIES = libgislanguage.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir) \
	-DDATADIR=\"$(datadir)\" \
	-DGNOMELOCALEDIR=\"$(datadir)/locale\"

BUILT_SOURCES = \
	language-resources.c \
	language-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/language.gresource.xml)
language-resources.c: language.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
language-resources.h: language.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgislanguage_la_SOURCES =	\
	$(BUILT_SOURCES) \
	gis-language-page.h	\
	gis-language-page.c \
	cc-language-chooser.h \
	cc-language-chooser.c \
	cc-util.h \
	cc-util.c \
	cc-common-language.h \
	cc-common-language.c

libgislanguage_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(FONTCONFIG_CFLAGS) \
	$(GNOME_DESKTOP_CFLAGS)

libgislanguage_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(FONTCONFIG_LIBS) \
	$(GNOME_DESKTOP_LIBS)

libgislanguage_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

EXTRA_DIST = \
	language.gresource.xml \
	$(resource_files)

CLEANFILES = $(BUILT_SOURCES)
