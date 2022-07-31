layoutsdir = join_paths(datadir, 'layouts')

debug('layout: datadir:', datadir)
debug('layout: layoutsdir:', layoutsdir)

layout_DATA = ['florence.xml', 'florence-alt.xml', 'compact.xml', 'compact-alt.xml']

layout_compact =
  configure_file(
    input: 'compact.xml.in.in',
    output: 'compat.xml.in',
    configuration: conf)

layout_compact_alt =
  configure_file(
    input: 'compact-alt.xml.in.in',
    output: 'compat-alt.xml.in',
    configuration: conf)

layout_florence =
  configure_file(
    input: 'florence.xml.in.in',
    output: 'florence.xml.in',
    configuration: conf)

layout_florence_alt =
  configure_file(
    input: 'florence-alt.xml.in.in',
    output: 'florence-alt.xml.in',
    configuration: conf)

install_data(
  [layout_compact, layout_compact_alt, layout_florence, layout_florence_alt],
  rename: layout_DATA,
  install_dir: layoutsdir)