gnome = import('gnome')

# Surround the version in quotes to make it a C string
config_h = configure_file(output : 'config.h',
               configuration : conf)

# florence.server is an xml file defining bonobo endpoints.
# it's basically an old OLE/COM/KParts/DBus
# gnome implementation of bonobo is called `ORBit`

# TODO: migrate to GIO or DBus for bonobo functionality (if required)

# this file will need to be transformed twice
# once for project settings, and once for translations
florence_server_file = 'florence.server'

server_conf = configuration_data()
server_conf.set('BINDIR', bindir)

# ISSUE: not sure where this is meant to install to
# ISSUE: uncommenting this causes build issues
#florence_server = i18n.merge_file(
#  input: configure_file(
#    input: florence_server_file +'.in.in',
#    output: florence_server_file + '.in',
#    configuration: server_conf),
#  output: florence_server_file,
#  po_dir: po_dir,
#  type: 'xml',
#  install: true)


florence_pc = 'florence-1.0.pc'

pc_conf = configuration_data()

pc_conf.set('prefix', prefix)
pc_conf.set('exec_prefix', '${prefix}')
pc_conf.set('libdir', '${exec_prefix}/lib')
pc_conf.set('includedir', '${prefix}/include')
pc_conf.set('PACKAGE_VERSION', as_version)

pc = configure_file(input: florence_pc + '.in', output: florence_pc, configuration: pc_conf)

sources = [
    config_h,
    'controller.c',
    'florence.c',
    'fsm.c',
    'key.c',
    'keyboard.c',
    'layoutreader.c',
    'menu.c',
    'ramble.c',
    'service.c',
    'settings.c',
    'settings-window.c',
    'status.c',
    'style.c',
    'tools.c',
    'trace.c',
    'trayicon.c',
    'view.c',
    'xkeyboard.c'
]

src_inc = include_directories('.')

cc = meson.get_compiler('c')

deps = [
    dependency('gtk+-3.0'),
    dependency('xext'),
    dependency('gmodule-2.0'),
    dependency('cairo'),
    dependency('librsvg-2.0'),
    dependency('libxml-2.0'),
    dependency('gstreamer-1.0'),
    dependency('X11'),
    ramble,
    xkb,
    xtst,
    spi,
    xrecord
]

# build libflorence
subdir('lib')

florence = executable('florence', 'main.c', sources: sources, include_directories: [src_inc, lib_inc, data_inc], dependencies: deps, link_with: libflorence, install: true)