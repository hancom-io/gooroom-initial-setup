noinst_LTLIBRARIES = libgissummary.la

libexec_SCRIPTS = gis-delete-lightdm-config-helper

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_builddir) \
	-DLOCALEDIR=\"$(localedir)\" \
	-DGIS_ADDUSER_HELPER=\"$(libexecdir)/gis-adduser-helper\" \
	-DGIS_COPY_WORKER=\"$(libexecdir)/gis-copy-worker\" \
	-DGIS_DELETE_LIGHTDM_CONFIG_HELPER=\"$(libexecdir)/gis-delete-lightdm-config-helper\"

BUILT_SOURCES = \
	summary-resources.c \
	summary-resources.h

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/summary.gresource.xml)
summary-resources.c: summary.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source $<
summary-resources.h: summary.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-header $<

libgissummary_la_SOURCES = \
	$(BUILT_SOURCES) \
	gis-summary-page.h \
	gis-summary-page.c \
	splash-window.h \
	splash-window.c \
	run-su.h \
	run-su.c

libgissummary_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GNOME_DESKTOP_CFLAGS)

libgissummary_la_LIBADD = \
	$(GTK_LIBS) \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(GNOME_DESKTOP_LIBS)

libgissummary_la_LDFLAGS = -export_dynamic -avoid-version -module -no-undefined

libexec_PROGRAMS = \
	gis-adduser-helper \
	gis-copy-worker

gis_adduser_helper_SOURCES = \
	gis-adduser-helper.c \
	run-passwd.h \
	run-passwd.c

gis_adduser_helper_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS)

gis_adduser_helper_LDFLAGS = \
	$(GLIB_LIBS) \
	$(GIO_LIBS)

gis_copy_worker_SOURCES = \
	gis-copy-worker.c

gis_copy_worker_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS)

gis_copy_worker_LDADD = \
	$(GLIB_LIBS) \
	$(GIO_LIBS)

EXTRA_DIST = \
	summary.gresource.xml \
	$(resource_files) \
	$(NULL)

CLEANFILES = $(BUILT_SOURCES)
