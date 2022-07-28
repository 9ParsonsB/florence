project('florence', 'c', version: '0.6.4')

top_inc = include_directories('.')

as_version = meson.project_version()

datadirname = 'share'
pkg_name = meson.project_name()

icondir = join_paths(get_option('prefix'), datadirname, 'pixmaps')
prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
opt_datadir = get_option('datadir')
mandir = get_option('mandir')
datadir = join_paths(prefix, opt_datadir, pkg_name)
localedir = join_paths(prefix, get_option('localedir'))
desktopdir = join_paths(datadir, 'applications')
pkgdatadir = join_paths(datadir, pkg_name)
po_dir = join_paths(meson.project_source_root(), 'po')
datarootdir = join_paths(prefix, datadirname)
docdir = join_paths(datarootdir, '/doc/', pkg_name)
florencelocaledir = join_paths(prefix, datadirname, '/locale')

conf = configuration_data()
conf.set_quoted('VERSION', as_version)
conf.set_quoted('LC_ALL', 'C')
conf.set_quoted('LOCALEDIR', localedir)
conf.set('HAVE_LIBM', 1)
conf.set('ENABLE_GTK3', 1)
conf.set('ENABLE_GTIO', 1)
conf.set_quoted('ICONDIR', icondir)
#conf.set('ICONDIR', '"data"')
conf.set_quoted('DATADIR', datadir )

debug('prefix:', prefix)
debug('datadir:', datadir)
debug('localedir:', localedir)
debug('icondir:', icondir)
debug('mandir:', mandir)
debug('florencelocaledir:', florencelocaledir)
debug('datarootdir:', datarootdir)


#ICONDIR=${prefix}/share/pixmaps

# Options
if get_option('with-docs')
    # TODO: add dependency scrollkeepr-config
    # set ENABLE_HELP
    conf.set('ENABLE_HELP', 1)
    gnome = import('gnome')

    gnome.gtkdoc('florence')
endif

if get_option('with-notifications')
    notify = dependency('libnotify')
    conf.set('ENABLE_NOTIFICATION', 1)
    conf.set('ENABLE_NOTIFICATION_ICON', 1)
else
    notify = dependency('libnotify', required: false)
endif

if get_option('with-xtst')
    xtst = dependency('xtst')
    conf.set('ENABLE_XTST', 1)
    if get_option('with-xrecord')
        xrecord = dependency('xrecord')
        config.set('ENABLE_XRECORD', 1)
    else
        xrecord = dependency('xrecord', required: false)
    endif
else
    xrecord = dependency('xrecord', required: false)
    xtst = dependency('xtst', required: false)
endif

if get_option('with-at-spi')
    spi = dependency('atspi-2')
    conf.set('ENABLE_AT_SPI2', 1)
    else
    spi = dependency('atspi-2', required: false)
endif

# TODO: test if both xtst and at-spi are disabled: ERROR

if get_option('with-xkb')
    xkb = dependency('xkbcommon')
    conf.set('ENABLE_XKB', 1)
else
    xkb = dependency('xkbcommon', required : false)
endif

if get_option('with-ramble')
  ramble = dependency('ramble')
  conf.set('ENABLE_RAMBLE', 1)
else
  ramble = dependency('ramble', required : false)
endif


# run meson.build in these dirs, in order

# translations
subdir('po')

# runtime & install data
subdir('data')

# source code
subdir('src')

# documentation
subdir('docs')

