AC_INIT([Florence virtual keyboard], [0.6.4], [f.agrech@gmail.com], [florence])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE(no-dist-gzip dist-bzip2)
LT_INIT

# Options
AC_ARG_WITH([docs], AS_HELP_STRING([--without-docs], [Build without docs (default: enabled)]))
AM_CONDITIONAL([WITH_DOCS], [test "x$with_docs" != "xno"])
AC_ARG_WITH([notification], AS_HELP_STRING([--without-notification], [Build without notification (default: enabled)]))
AM_CONDITIONAL([WITH_NOTIFICATION], [test "x$with_notification" != "xno"])
AC_ARG_WITH([xtst], AS_HELP_STRING([--without-xtst], [Build without Xtst extension (default: enabled)]))
AM_CONDITIONAL([WITH_XTST], [test "x$with_xtst" != "xno"])
AC_ARG_WITH([xrecord], AS_HELP_STRING([--without-xrecord], [Build without XRecord extension (requires the Xtst extension) (default: enabled)]))
AM_CONDITIONAL([WITH_XRECORD], [test "x$with_xrecord" != "xno" -a "x$with_xtst" != "xno"])
AC_ARG_WITH([at-spi], AS_HELP_STRING([--without-at-spi], [Build without at-spi(default: enabled)]))
AM_CONDITIONAL([WITH_AT_SPI], [test "x$with_at_spi" != "xno"])
AC_ARG_WITH([panelapplet], AS_HELP_STRING([--without-panelapplet], [Build without libpanelapplet(default: enabled)]))
if test "x$with_xtst" = "xno" -a  "x$with_at_spi" = "xno"; then
	AC_MSG_ERROR(You can't disable both xtst and at-spi as there is no way Florence can generate keyboard events.)
fi
AC_ARG_WITH([xkb], AS_HELP_STRING([--without-xkb], [Build without xkb(default: enabled)]))
AM_CONDITIONAL([WITH_XKB], [test "x$with_xkb" != "xno"])
if test "x$with_xkb" != "xno"; then
	AC_DEFINE(ENABLE_XKB, ["xkb"], [xkb extension is enabled])
fi
AC_ARG_WITH([ramble], AS_HELP_STRING([--without-ramble], [Build without ramble mode (default: enabled)]))
AM_CONDITIONAL([WITH_RAMBLE], [test "x$with_ramble" != "xno"])
if test "x$with_ramble" != "xno"; then
	AC_DEFINE(ENABLE_RAMBLE, ["ramble"], [Ramble mode is enabled])
fi

# Internationalization
AC_PROG_INTLTOOL([0.23])
GETTEXT_PACKAGE=florence
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE(GETTEXT_PACKAGE, ["florence"], [Package name for gettext])
ALL_LINGUAS="fr ru it de"
AM_GLIB_GNU_GETTEXT
florencelocaledir='${prefix}/${DATADIRNAME}/locale'
AC_SUBST(florencelocaledir)

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_GREP
AC_PROG_SED
PKG_PROG_PKG_CONFIG
GLIB_GSETTINGS

GNOME_DOC_INIT(,,
	if test "x$with_docs" != "xno"; then
		AC_MSG_ERROR([gnome-doc-utils >= $gdu_cv_version_required not found])
	fi)
if test "x$with_docs" != "xno"; then
	AC_PATH_PROG(SK_CONFIG,scrollkeeper-config,no)
	if test x$SK_CONFIG = xno; then
		AC_MSG_ERROR(Couldn't find scrollkeeper-config. Please install the scrollkeeper package: http://scrollkeeper.sourceforge.net)
	fi
	SCROLLKEEPER_BUILD_REQUIRED=0.3.5
	AC_SUBST(SCROLLKEEPER_BUILD_REQUIRED)
fi

# Checks for libraries.
DEP_MODULES="xext gmodule-2.0 cairo librsvg-2.0 libxml-2.0 gstreamer-1.0"
PKG_CHECK_MODULES(DEPS, $DEP_MODULES)

PKG_CHECK_MODULES([GTK3], [gtk+-3.0], AC_DEFINE([ENABLE_GTK3], [], [GTK3 enabled.]),
	AC_MSG_ERROR(gtk version 3.0 or superior required. Please install the dev package of gtk))

PKG_CHECK_MODULES([GIO], [gio-2.0], AC_DEFINE([ENABLE_GIO], [], [GIO enabled.]),
	AC_MSG_ERROR(GIO version 2.0 or superior required. Please install the dev package of gtk))

if test "x$with_docs" != "xno"; then
	PKG_CHECK_MODULES([GNOME_DOC_UTILS], [gnome-doc-utils], AC_DEFINE([ENABLE_HELP], [], [Help enabled.]), AC_MSG_ERROR(Couldn't find gnome-doc-utils.
Please either install gnome-doc-utils package or disable docs: --without-docs configure option))
fi
if test "x$with_notification" != "xno"; then
	PKG_CHECK_MODULES([LIBNOTIFY], [libnotify], AC_DEFINE([ENABLE_NOTIFICATION], [], [Notification enabled.]), AC_MSG_ERROR(Could not configure libnotify. Please either install libnotify or disable notifications: --without-notification configure option))
	PKG_CHECK_MODULES([LIBNOTIFY_ICON], [libnotify < 0.7.0], AC_DEFINE([ENABLE_NOTIFICATION_ICON], [], [Notification icon enabled.]), AC_MSG_NOTICE([Notification icon disabled.]))
fi

if test "x$with_xtst" != "xno"; then
	PKG_CHECK_MODULES([XTST], [xtst], AC_DEFINE([ENABLE_XTST], [], [Xtst extension enabled.]), AC_MSG_ERROR(Could not configure xtst. Please either install xtst or disable it: --without-xtst configure option))
	if test "x$with_xrecord" != "xno"; then
		AC_MSG_WARN([Xorg RECORD extension is severely broken in version 1.6 of Xorg: see bugs http://bugs.freedesktop.org/show_bug.cgi?id=20500 and http://bugs.freedesktop.org/show_bug.cgi?id=21971 ; Please disable xrecord if you affected by this bug: --without-xrecord option])
		AC_DEFINE(ENABLE_XRECORD, ["xrecord"], [xrecord extension is enabled])
	fi
fi
if test "x$with_at_spi" != "xno"; then
	PKG_CHECK_MODULES([AT_SPI2], [atspi-2], AC_DEFINE([ENABLE_AT_SPI2], [], [dbus at-spi enabled.]),
		AC_MSG_ERROR(Could not configure at-spi. Please either install at-spi or disable it: --without-at-spi configure option))
fi

# Math library
AC_CHECK_LIB([m], [tan], AC_DEFINE(HAVE_LIBM, 1, Math lib) LIBM="-lm", AC_MSG_ERROR(Missing working libm math lib))

# X11 library
PKG_CHECK_MODULES([X11], [x11])

AC_SUBST(GNOME_DOCS_UTILS_CFLAGS)
AC_SUBST(GNOME_DOCS_UTILS_LIBS)
AC_SUBST(LIBNOTIFY_CFLAGS)
AC_SUBST(LIBNOTIFY_LIBS)
AC_SUBST(XTST_CFLAGS)
AC_SUBST(XTST_LIBS)
AC_SUBST(AT_SPI2_CFLAGS)
AC_SUBST(AT_SPI2_LIBS)
AC_SUBST(LIBGNOME_CFLAGS)
AC_SUBST(LIBGNOME_LIBS)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)
AC_SUBST(LIBM)

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_MAJOR
AC_CHECK_HEADERS([math.h fcntl.h libintl.h locale.h memory.h stdio.h stdlib.h string.h strings.h sys/param.h sys/stat.h sys/types.h sys/file.h sys/time.h utime.h dirent.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STAT
AC_C_CONST
AC_CHECK_MEMBERS([struct stat.st_blksize])

# Checks for library functions.
AC_TYPE_SIZE_T
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_CHECK_FUNCS([setlocale memset memchr sscanf strcmp strrchr strchr strstr strcpy strncpy strcat atoi putenv fopen fclose fgets fputs lstat opendir readdir closedir])

# Files generated
ICONDIR=${prefix}/share/icons/hicolor/scalable/apps
if test "x$prefix" = "xNONE"; then
	ICONDIR=/usr/local/share/icons/hicolor/scalable/apps
fi
AC_SUBST(ICONDIR)
AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/layouts/Makefile
                 data/relaxng/Makefile
                 data/styles/Makefile
                 data/styles/bright/Makefile
                 data/styles/default/Makefile
                 data/styles/default/sounds/Makefile
                 data/styles/hard/Makefile
                 data/styles/flat/Makefile
                 data/svg11/Makefile
                 docs/Makefile
                 docs/florence.1
                 docs/florence_applet.1
                 src/Makefile
                 src/florence-1.0.pc
                 po/Makefile.in])
AC_OUTPUT

