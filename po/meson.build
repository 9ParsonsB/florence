i18n = import('i18n')
po_conf = configuration_data()
potfiles = configure_file(input: 'POTFILES.in', output: 'POTFILES', configuration: po_conf)

# define GETTEXT_PACKAGE
add_project_arguments('-DGETTEXT_PACKAGE="intltest"', language:'c')
mo_targets = i18n.gettext(meson.project_name(), preset:'glib')