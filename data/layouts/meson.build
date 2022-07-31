layoutsdir = join_paths(datadir, 'layouts')

debug('layout: datadir:', datadir)
debug('layout: layoutsdir:', layoutsdir)

layout_DATA = ['florence.xml', 'florence-alt.xml', 'compact.xml', 'compact-alt.xml']

foreach i: layout_DATA
configure_file(
  input: i18n.merge_file(
          input:i + '.in.in',
          output: i + '.in',
          po_dir: po_dir,
         ).full_path(),
  output: i,
  configuration: conf,
  install: true,
  install_dir: layoutsdir
)
endforeach