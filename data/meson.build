gnome = import('gnome')
schemadir = join_paths(datadir, 'glib-2.0', 'schemas')

data_inc = include_directories('.')
data_conf = configuration_data()
data_xml = 'org.florence.gschema.xml'

florence_schema_xml = configure_file(input: data_xml + '.in', output: data_xml, configuration: data_conf)

install_data(florence_schema_xml, install_dir: schemadir)

#doc_resources = gnome.compile_resources('florence', data_xml, c_name:'florence')