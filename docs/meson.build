doc_figures = files(
  'figures/behaviour.png',
  'figures/florence.png',
  'figures/layout.png',
  'figures/style.png',
  'figures/window.png',
  'figures/minimize.png',
  'figures/configuration.png',
  'figures/move.png',
  'figures/bigger.png',
  'figures/smaller.png',
  'figures/close.png',
  'figures/switch.png',)


man_florence_file = 'florence.1'
man_florece_applet_file = 'florence_applet.1'

man_conf = configuration_data()

man_florence = configure_file(input: man_florence_file + '.in', output: man_florence_file, conf: man_conf)
man_florence_applet = configure_file(input: man_florece_applet_file + '.in', output: man_florece_applet_file, conf: man_conf)

man_files = [man_florence, man_florence_applet]

install_data(man_files, install_dir: mandir)