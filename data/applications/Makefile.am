desktop_in_in_files = \
	gooroom-initial-setup.desktop.in.in

desktopdir = $(datadir)/applications
desktop_in_files = $(desktop_in_in_files:.desktop.in.in=.desktop.in)
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

%.desktop.in: %.desktop.in.in
	$(AM_V_GEN) sed -e 's,[@]libexecdir[@],$(libexecdir),g' < $< > $@

%.desktop: %.desktop.in
	$(AM_V_GEN) msgfmt --desktop -d $(top_srcdir)/po --template $< -o $@

EXTRA_DIST = \
	$(desktop_in_in_files)

CLEANFILES = \
	$(desktop_in_files) \
	$(desktop_DATA)
