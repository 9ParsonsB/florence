gnome = import('gnome')

schemadir = join_paths(datarootdir, 'glib-2.0', 'schemas')

data_inc = include_directories('.')
data_conf = configuration_data()
data_xml = 'org.florence.gschema.xml'

debug('podir:', po_dir)

# gschema xml is transformed using merge_file (for translations) before being installed
florence_schema_xml = i18n.merge_file(
  input: data_xml + '.in',
  output: data_xml,
  po_dir: po_dir,
  install: true,
  install_dir: schemadir
)

#florence_schema_xml = configure_file(input: data_xml + '.in', output: data_xml, configuration: data_conf)

#compile the schema in the current directory, forw when running in debug mode
gnome.compile_schemas(build_by_default: true, depend_files: data_xml)

#install gnome settings schema to system
meson.add_install_script('glib-compile-schemas', schemadir)

doc_resources = gnome.compile_resources('florence', data_xml, c_name:'florence' )

subdir('layouts')