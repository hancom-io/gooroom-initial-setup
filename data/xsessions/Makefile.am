xsession_in_in_files = \
	gooroom-initial-setup.desktop.in

xsessiondir = $(datadir)/xsessions
xsession_in_files = $(xsession_in_in_files:.desktop.in.in=.desktop.in)
xsession_DATA = $(xsession_in_files:.desktop.in=.desktop)

%.desktop.in: %.desktop.in.in
	$(AM_V_GEN) sed -e 's,[@]libexecdir[@],$(libexecdir),g' < $< > $@

%.desktop: %.desktop.in
	$(AM_V_GEN) msgfmt --desktop -d $(top_srcdir)/po --template $< -o $@

EXTRA_DIST = \
	$(xsession_in_in_files)

CLEANFILES = \
	$(xsession_in_files) \
	$(xsession_DATA)
