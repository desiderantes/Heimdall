heimdall_qt_sources = files (
    'source/aboutform.cpp',
    'source/Alerts.cpp',
    'source/FirmwareInfo.cpp',
    'source/main.cpp',
    'source/mainwindow.cpp',
    'source/PackageData.cpp',
    'source/Packaging.cpp',
)

heimdall_qt_headers = files(
    'source/Alerts.h',
    'source/FirmwareInfo.h',
    'source/PackageData.h',
    'source/Packaging.h',
)

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : ['Core', 'Gui', 'Widgets'])

heimdall_qt_sources += qt5.preprocess(
    moc_headers: files('source/aboutform.h', 'source/mainwindow.h',),
    ui_files: files('aboutform.ui', 'mainwindow.ui',),
    qresources: files('mainwindow.qrc',),
)

heimdall_qt_sources += conf_header


heimdall_qt_bin = executable('heimdall-qt5',
                             sources : heimdall_qt_sources,
                             include_directories : [conf_inc_dir, libpit_inc_dir, heimdall_inc_dir],
                             dependencies : [libusb_dep, thread_dep, zlib_dep, qt5_dep],
                             link_with : [heimdall_lib, libpit_dep],
                             install : true,
)