
style_default_sounds_dir = default_style_dir / 'sounds'

sounds_xml = 'sounds.xml'
sound_conf = configuration_data()
sound_conf.set('SNDDIR', style_default_sounds_dir)

configure_file(
  input: sounds_xml + '.in',
  output: sounds_xml,
  configuration: sound_conf,
  install: true,
  install_dir: style_default_sounds_dir)

default_sound_files = files(
  'press.ogg', 'release.ogg', 'return_press.ogg',
  'return_release.ogg', 'modifier_press.ogg', 'modifier_release.ogg')

install_data(default_sound_files, install_dir: style_default_sounds_dir)