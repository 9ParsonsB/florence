lib_headers = files('florence.h')

lib_deps = [dependency('gtk+-3.0')]
lib_inc = include_directories('.')
lib_src = files('florence.c')

libflorence = library('florence', version: '1.0', sources: lib_src, include_directories: [lib_inc, src_inc], dependencies: lib_deps, install: true)