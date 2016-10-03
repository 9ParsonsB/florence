
SUBDIRS=styles layouts relaxng svg11

gsettings_in_files=org.florence.gschema.xml.in
@INTLTOOL_XML_NOMERGE_RULE@
gsettings_SCHEMAS=$(gsettings_in_files:.gschema.xml.in=.gschema.xml)
@GSETTINGS_RULES@

desktopdir=$(datadir)/applications
desktop_in_files=florence.desktop.in
desktop_DATA=$(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@
$(desktop_in_files): $(desktop_in_files:.desktop.in=.desktop.in.in)
	sed -e "s|\@ICONDIR\@|$(ICONDIR)|" $< > $@

iconsdir=$(datadir)/icons/hicolor/scalable/apps
icons_DATA=florence.svg

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
	  echo "Updating GTK icon cache."; \
	  $(gtk_update_icon_cache); \
	else \
	  echo "*** Icon cache not updated. After (un)install, run this:"; \
	  echo "*** $(gtk_update_icon_cache)"; \
	fi

gtkbuilderdir=$(datadir)/florence
gtkbuilder_DATA=florence.glade

metadir=$(datadir)/florence
meta_DATA=florence.rnc florence.css

configdir=$(datadir)/florence
config_DATA=florence.conf

EXTRA_DIST = $(config_DATA) $(gtkbuilder_DATA) $(icons_DATA) $(meta_DATA)\
	     $(gsettings_in_files) $(desktop_in_files:.desktop.in=.desktop.in.in)
DISTCLEANFILES = $(desktop_DATA) $(desktop_in_files) $(desktop_DATA) $(gsettings_SCHEMAS)

