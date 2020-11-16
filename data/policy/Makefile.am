kr.gooroom.InitialSetup.policy.in: kr.gooroom.InitialSetup.policy.in.in Makefile
	$(AM_V_GEN) sed -e "s|\@libexecdir\@|$(libexecdir)|" $< >$@

@INTLTOOL_POLICY_RULE@
polkitdir       = $(datadir)/polkit-1/actions
polkit_in_files = kr.gooroom.InitialSetup.policy.in
polkit_DATA     = $(polkit_in_files:.policy.in=.policy)

CLEANFILES = \
	kr.gooroom.InitialSetup.policy \
	kr.gooroom.InitialSetup.policy.in

DISTCLEANFILES = Makefile.in
