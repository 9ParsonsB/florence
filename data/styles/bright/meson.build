
style_bright_svg = 'default.svg'

style_bright_dir = styledir / 'bright'

configure_file(
  input: style_bright_svg + '.in',
  output: style_bright_svg,
  configuration: svg_conf,
  install: true,
  install_dir: style_bright_dir)

style_bright_files = files(pkg_name + '.defs', pkg_name + '.style')

install_data(style_bright_files, install_dir: style_bright_dir)