gnome = import('gnome')

# Surround the version in quotes to make it a C string
config_h = configure_file(output : 'config.h',
               configuration : conf)

florence_pc = 'florence-1.0.pc'

pc_conf = configuration_data()

pc_conf.set('@prefix@', prefix)
pc_conf.set('@exec_prefix@', '${prefix}')
pc_conf.set('@libdir@', '${exec_prefix}/lib')
pc_conf.set('@includedir@', '${prefix}/include')
pc_conf.set('@PACKAGE_VERSION@', version)

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