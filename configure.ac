AC_INIT(gooroom-initial-setup, 0.1.0)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE([no-dist-gzip dist-xz foreign])
LT_INIT
AM_PROG_CC_C_O
AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])

AC_ARG_WITH(
	[pamdir],
	[AS_HELP_STRING([--with-pamdir=PATH],[Specify the directory where PAM modules are stored])],
	[pamdir="${withval}"],
	[
		if test "${prefix}" = "/usr"; then
			pamdir="/lib${libdir##*/lib}/security"
		else
			pamdir="\$(libdir)/security"
		fi
	]
)
AC_SUBST([pamdir])

dnl ###########################################################################
dnl Dependencies
dnl ###########################################################################
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GIO], [gio-2.0])
PKG_CHECK_MODULES([PANGO], pango >= 1.32.5)
PKG_CHECK_MODULES(LIBNM, libnm >= 1.2)
PKG_CHECK_MODULES(LIBNMA, libnma >= 1.0)
PKG_CHECK_MODULES(GOA, [goa-1.0])
PKG_CHECK_MODULES(GOA_BACKEND, [goa-backend-1.0])
PKG_CHECK_MODULES(LIBSECRET, [libsecret-1])
PKG_CHECK_MODULES(PWQUALITY, [pwquality])
PKG_CHECK_MODULES(FONTCONFIG, fontconfig)
PKG_CHECK_MODULES(GNOME_DESKTOP, gnome-desktop-3.0 >= 3.30.2.1)


dnl ###########################################################################
dnl Internationalization
dnl ###########################################################################

IT_PROG_INTLTOOL(0.35.0)
AC_SUBST(GETTEXT_PACKAGE, gooroom-initial-setup)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", Gettext package)

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_CONFIG_FILES([
Makefile
po/Makefile.in
data/Makefile
data/images/Makefile
data/policy/Makefile
data/sessions/Makefile
data/xsessions/Makefile
data/applications/Makefile
src/Makefile
src/pages/Makefile
src/pages/language/Makefile
src/pages/eulas/Makefile
src/pages/account/Makefile
src/pages/network/Makefile
src/pages/goa/Makefile
src/pages/summary/Makefile
])
AC_OUTPUT
