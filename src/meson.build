gnome = import('gnome')

# Surround the version in quotes to make it a C string
config_h = configure_file(output : 'config.h',
               configuration : conf)

sources = [
    config_h,
    'src/main.c',
    'src/controller.c',
    'src/florence.c',
    'src/fsm.c',
    'src/key.c',
    'src/keyboard.c',
    'src/layoutreader.c',
    'src/menu.c',
    'src/ramble.c',
    'src/service.c',
    'src/status.c',
    'src/style.c',
    'src/tools.c',
    'src/trace.c',
    'src/trayicon.c',
    'src/view.c',
    'src/xkeyboard.c'
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
    # dependency('x11'),
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

florence = executable('florence', sources: sources, include_directories: [src_inc], dependencies: deps, link_with: libflorence, install: true)