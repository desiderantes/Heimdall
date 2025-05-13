project('heimdall', ['cpp'], version : '1.4.2',
        license : 'MIT',
        default_options : ['cpp_std=c++23']
)

cpp = meson.get_compiler('cpp')

thread_dep = dependency('threads')
libusb_dep = dependency('libusb-1.0')
zlib_dep = dependency('zlib', required : true)

conf_data = configuration_data()
conf_data.set_quoted('HEIMDALL_VERSION', meson.project_version())
conf_data.set_quoted('LIBUSB_VERSION', libusb_dep.version())
conf_data.set_quoted('ZLIB_VERSION', zlib_dep.version())
conf_header = configure_file(
    output : 'heimdall_config.h',
    configuration : conf_data,
)

conf_inc_dir = include_directories('.')

subdir('libpit')
subdir('heimdall')
qt_gui_opt = get_option('enable_qt_frontend')
message('Qt GUI enabled:', qt_gui_opt.enabled())
if qt_gui_opt.enabled()
    subdir('heimdall-frontend')
endif

if host_machine.system() == 'linux'
    subdir('linux')
elif host_machine.system() == 'macos'
    subdir('osx')
else
    error('Unsupported platform')
endif