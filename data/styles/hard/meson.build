style_hard_dir = styledir / 'hard'

style_hard_in =
  ['default.svg',
  'return.svg',
  'space.svg',
  'wide.svg',
  'xl.svg',
  'xxl.svg',
  'high.svg',
  'tiny.svg',
  'mini.svg',
  'small.svg']

style_hard_conf = configuration_data()
style_hard_conf.set('BASEDIR' / style_hard_dir)

foreach i: style_hard_in
  configure_file(
    input: i + '.in',
    output: i,
    configuration: style_hard_conf,
    install: true,
    install_dir: style_hard_dir)
endforeach

install_data(files(pkg_name + '.style', pkg_name + '.defs'), install_dir: style_hard_dir)