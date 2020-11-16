SUBDIRS = \
	images \
	policy \
	sessions \
	xsessions \
	applications

libexec_SCRIPTS = \
	gooroom-initial-setup-proxy

polkit_rulesdir = $(datadir)/polkit-1/rules.d
polkit_rules_DATA = 50_gooroom-initial-setup.rules

lightdm_confdir = $(sysconfdir)/lightdm/lightdm.conf.d
lightdm_conf_DATA = \
	90_gooroom-initial-setup.conf

EXTRA_DIST =                        \
	50_gooroom-initial-setup.rules   
