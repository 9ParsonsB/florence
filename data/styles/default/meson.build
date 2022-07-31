default_style_dir = styledir / 'default'

style_symbol_file = 'smybols.xml'
style_default_style = pkg_name + '.style'

default_style_conf = configuration_data()
default_style_conf.set('ICONDIR', icondir)
default_style_conf.set('BASEDIR', default_style_dir)

configure_file(
  input: style_default_style + '.in',
  output: style_default_style,
  configuration: default_style_conf,
  install: true,
  install_dir: default_style_dir)

i18n.merge_file(
  input: style_symbol_file + '.in',
  output: style_symbol_file,
  po_dir: po_dir,
  install: true,
  install_dir: default_style_dir
)

install_data(pkg_name + '.defs', install_dir: default_style_dir)

subdir('sounds')