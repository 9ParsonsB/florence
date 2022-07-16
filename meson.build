project('florence', 'c', version: '0.6.4')
conf = configuration_data()

as_version = meson.project_version()
conf.set_quoted('VERSION', as_version)
conf.set_quoted('LC_ALL', 'C')

conf.set('HAVE_LIBM', 1)
conf.set('ENABLE_GTK3', 1)
conf.set('ENABLE_GTIO', 1)

# Options

if get_option('with-docs')
    # TODO: add dependency scrollkeepr-config
    # set ENABLE_HELP
    config.set('ENABLE_HELP', 1)
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
endif



if get_option('with-at-spi')
    spi = dependency('atspi-2')
    conf.set('ENABLE_AT_SPI2', 1)
    else
    spi = dependency('atspi-2', required: false)
endif


# TODO: test if both xtst and at-spi are disabled: ERROR

if get_option('with-xkb')
    xkb = dependency('xkb')
    conf.set('ENABLE_XKB', 1)
else
    xkb = dependency('xkb', required : false)
endif


if get_option('with-ramble')
  ramble = dependency('ramble')
  conf.set('ENABLE_RAMBLE', 1)
else
  ramble = dependency('ramble', required : false)
endif

# I18n
#i18n = import('i18n')
#i18n.merge_file(po_dir: 'po', output:)
#i18n.gettext('docs', data_dirs: ['${prefix}/${DATADIRNAME}/locale'], languages: ['fr', 'ru', 'it', 'de'])






# Surround the version in quotes to make it a C string
configure_file(output : 'config.h',
               configuration : conf)

florencedep = [dependency('gtk+-3.0'), dependency('xext'), dependency('gmodule-2.0'), dependency('cairo'), dependency('librsvg-2.0'), dependency('libxml-2.0'), dependency('gstreamer-1.0'), ramble, xkb]

executable('florence', 'src/main.c', dependencies : florencedep)