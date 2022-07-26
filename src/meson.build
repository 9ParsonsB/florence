gnome = import('gnome')

# Surround the version in quotes to make it a C string
config_h = configure_file(output : 'config.h',
               configuration : conf)

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


cflags = [
  '-DICONDIR="@0@"'.format(icondir),
  '-DFLORENCELOCALEDIR="@0@"'.format(florencelocaledir),
  '-DDATADIR="@0@"'.format(datadir),
  ]

# build libflorence
subdir('lib')

florence = executable('florence', 'main.c', sources: sources, include_directories: [src_inc, lib_inc], dependencies: deps, link_with: libflorence, install: true)