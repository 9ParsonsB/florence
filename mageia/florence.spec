Name:           florence
Version:        0.6.0
Release:        %mkrel 2
Summary:        Extensible scalable on-screen virtual keyboard
Group:          System/X11
License:        GPLv2+ and GFDL
URL:            http://florence.sourceforge.net
Source0:        http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.bz2
Patch:		%{name}-glibh.patch

BuildRequires:    gtk3-devel
BuildRequires:    libxml2-devel
BuildRequires:    libglade2-devel
BuildRequires:    at-spi-devel
BuildRequires:    librsvg2-devel
BuildRequires:    cairo-devel
BuildRequires:    libgnome2-devel
BuildRequires:    libGConf2-devel
BuildRequires:    desktop-file-utils
BuildRequires:    scrollkeeper
BuildRequires:    intltool
BuildRequires:    libnotify-devel
BuildRequires:    gnome-doc-utils
BuildRequires:    libxtst-devel
BuildRequires:    pkgconfig(gstreamer-0.10)

%description
Florence is an extensible scalable virtual keyboard. 

It's designed for GNOME but can be used in any DE (without some features).

You need it if you can't use a real hardware keyboard, for 
example because you are disabled, your keyboard is broken or 
because you use a tablet PC, but you must be able to use a pointing 
device (as a mouse, a trackball or a touchscreen).

In GNOME Florence stays out of your way when you don't need it:
it appears on the screen only when you need it. 

A Timer-based auto-click functionality is available 
to help disabled people having difficulties to click.

If you have problems switching keyboard layout (language) in KDE4,
reconfigure layouts in KDE4 settings once and it should work.

%prep
%setup -q
#apply_patches

rm -f gconf-refresh
ln -sf /bin/true gconf-refresh

sed -i 's|Icon=%{name}.svg|Icon=%{name}|g' data/%{name}.desktop.in.in


%build
%configure \
      --without-panelapplet \
      --without-xrecord \
      --without-notification \
      --disable-schemas-install
%make


%install
rm -rf %{buildroot}

%makeinstall_std

desktop-file-install \
        --delete-original \
        --remove-category="Application" \
        --add-category="Utility" \
        --dir=%{buildroot}%{_datadir}/applications \
        %{buildroot}/%{_datadir}/applications/%{name}.desktop

mkdir -p %{buildroot}%{_datadir}/icons/hicolor/scalable/apps/ 

install -p -m 0644 data/%{name}.svg \
    %{buildroot}%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%find_lang %{name}

%files -f %{name}.lang
%doc AUTHORS ChangeLog COPYING COPYING-DOCS NEWS README 
%{_datadir}/%{name}/
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.*
%{_datadir}/gnome/help/%{name}/
%{_datadir}/omf/%{name}/
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/glib-2.0/schemas/org.florence.gschema.xml
%{_mandir}/man1/florence.1.*
%{_mandir}/man1/florence_applet.1.*


%changelog
* Tue Oct 22 2013 umeabot <umeabot> 0.6.0-2.mga4
+ Revision: 541352
- Mageia 4 Mass Rebuild

* Sat Oct 12 2013 matteo <matteo> 0.6.0-1.mga4
+ Revision: 495648
- new version 0.6.0

* Fri Jan 11 2013 umeabot <umeabot> 0.5.1-3.mga3
+ Revision: 350556
- Mass Rebuild - https://wiki.mageia.org/en/Feature:Mageia3MassRebuild

* Thu Jan 05 2012 matteo <matteo> 0.5.1-2.mga2
+ Revision: 191898
- fixed bug about glib.h inclusion

* Tue Oct 11 2011 matteo <matteo> 0.5.1-1.mga2
+ Revision: 154001
- imported package florence
