libflorence_inc = include_directories('.')

headers = files('florence.h')

lib_deps = [dependency('gtk+-3.0')]

sources = files('florence.c')

libflorence = shared_library('libflorence', version: '1.0', sources: sources, include_directories: top_inc, dependencies: lib_deps, install: true)