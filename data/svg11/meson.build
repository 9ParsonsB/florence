svg11_dir = datadir / 'svg11'

svg11_files = [
  'svg-basic-graphics-attrib', 'svg-docevents-attrib',
  'svg-image', 'svg-structure', 'svg-basic-structure',
  'svg-extensibility', 'svg-marker', 'svg-style',
  'svg-basic-text', 'svg-extresources-attrib', 'svg-mask',
  'svg-text', 'svg11', 'svg-clip', 'svg-filter',
  'svg-opacity-attrib', 'svg-viewport-attrib',
  'svg-animation', 'svg-conditional', 'svg-font',
  'svg-paint-attrib', 'svg-view', 'svg-animevents-attrib',
  'svg-container-attrib', 'svg-gradient', 'svg-pattern',
  'svg-xlink-attrib', 'svg-basic-clip', 'svg-core-attrib',
  'svg-graphevents-attrib', 'svg-profile', 'svg-basic-filter',
  'svg-cursor', 'svg-graphics-attrib', 'svg-script',
  'svg-basic-font', 'svg-datatypes', 'svg-hyperlink',
  'svg-shape']

foreach i: svg11_files
  custom_target(
    'gen-svg11-rng-' + i,
      input: i + '.rnc',
      output: i + '.rng',
      command: [trang_command, '-I rnc', '-O rng', '@INPUT@', '@OUTPUT@'],
      install: true,
      install_dir: svg11_dir)
endforeach

install_data('README', install_dir: svg11_dir)