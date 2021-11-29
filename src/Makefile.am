SUBDIRS = pages

libexec_PROGRAMS =  \
	gooroom-initial-setup

BUILT_SOURCES = \
	gis-resources.c \
	gis-resources.h

gooroom_initial_setup_SOURCES = \
	$(BUILT_SOURCES) \
	gis-keyring.h \
	gis-keyring.c \
	gis-main.c \
	gis-assistant.h \
	gis-assistant.c \
	gis-page.h \
	gis-page.c \
	gis-page-manager.h \
	gis-page-manager.c \
	gis-message-dialog.h \
	gis-message-dialog.c

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DHAVE_CONFIG_H \
	-DSECRET_API_SUBJECT_TO_CHANGE \
	-DLOCALEDIR=\"$(localedir)\" \
	-DPKGDATA_DIR=\"$(pkgdatadir)\" \
	$(WARN_CFLAGS)

gooroom_initial_setup_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(LIBSECRET_CFLAGS) \
	$(WEBKIT_CFLAGS)

gooroom_initial_setup_LDADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(LIBSECRET_LIBS) \
	$(WEBKIT_LIBS) \
	pages/language/libgislanguage.la \
	pages/eulas/libgiseulas.la \
	pages/network/libgisnetwork.la \
	pages/account/libgisaccount.la \
	pages/goa/libgisgoa.la \
	pages/summary/libgissummary.la

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/gresource.xml)
gis-resources.c: gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
gis-resources.h: gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

CLEANFILES = $(BUILT_SOURCES)
