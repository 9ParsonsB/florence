gnome = import('gnome')

schemadir = join_paths(datarootdir, 'glib-2.0', 'schemas')

data_inc = include_directories('.')
data_conf = configuration_data()
data_xml = namespace + '.gschema.xml'

debug('podir:', po_dir)

# gschema xml is transformed using merge_file (for translations) before being installed
# but it shouldnt be, so this is disabled. see: https://wiki.gnome.org/HowDoI/GSettings
#florence_schema_xml = i18n.merge_file(
#  input: data_xml + '.in',
#  output: data_xml,
#  po_dir: po_dir,
#  install: true,
#  install_dir: schemadir
#)

#florence_schema_xml = configure_file(input: data_xml + '.in', output: data_xml, configuration: data_conf)

#compile the schema in the current directory, for when running in debug mode
compile_schemas = gnome.compile_schemas(build_by_default: true)

florence_rng_file = pkg_name + '.rng'
florence_rnc_file = pkg_name + '.rnc'
trang_command = get_variable('TRANG_COMMAND', 'trang')

# install rng file to datadir / 'relaxng'
florence_rng = custom_target(
  'gen-florence-rng',
    input: florence_rnc_file,
    output: florence_rng_file,
    command: [trang_command, '-I rnc', '-O rng', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: datadir / 'relaxng')

# install schema to gio schemadir
install_data(
  [data_xml, florence_rnc_file],
  install_dir: schemadir,
)

# install gnome settings schema to system on install
#meson.add_install_script('glib-compile-schemas', schemadir)

subdir('layouts')
subdir('styles')
subdir('svg11')