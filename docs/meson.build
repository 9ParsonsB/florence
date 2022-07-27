doc_figures = files(
  'C/figures/behaviour.png',
  'C/figures/florence.png',
  'C/figures/layout.png',
  'C/figures/style.png',
  'C/figures/window.png',
  'C/figures/minimize.png',
  'C/figures/configuration.png',
  'C/figures/move.png',
  'C/figures/bigger.png',
  'C/figures/smaller.png',
  'C/figures/close.png',
  'C/figures/switch.png',)


man_florence_file = 'florence.1'
man_florece_applet_file = 'florence_applet.1'

man_conf = configuration_data()

man_florence = configure_file(input: man_florence_file + '.in', output: man_florence_file, configuration: man_conf)
man_florence_applet = configure_file(input: man_florece_applet_file + '.in', output: man_florece_applet_file, configuration: man_conf)

man_files = [man_florence, man_florence_applet]

install_data(man_files, install_dir: mandir)