udev_dep = dependency('udev')
udev_rules_dir = udev_dep.get_variable(pkgconfig:'udevdir')
install_data(files('udev/60-heimdall.rules'),
             install_dir: join_paths(udev_rules_dir, 'rules.d')
)