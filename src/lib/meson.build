headers = files('florence.h')

lib_deps = [dependency('gtk+-3.0')]

sources = files('florence.c')

libflorence = shared_library('libflorence', version: '1.0', sources: sources, include_directories: [include_directories('.')], dependencies: lib_deps, install: true)