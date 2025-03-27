project('dbus-cpp-connect', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

# Исходные файлы
common_sources = files([])

subdir('source')

server_sources = files([
  'examples/server.cpp',
])

client_sources = files([
  'examples/client.cpp',
])

dbus_dep = dependency('dbus-1')
incdir = include_directories('include')

# Исполняемые файлы
executable('dbus-server', 
          common_sources + server_sources, 
          include_directories : incdir,
          dependencies: [dbus_dep])

executable('dbus-client', 
          common_sources + client_sources,
          include_directories : incdir,
          dependencies: [dbus_dep])

executable('simple-sender',
          'simple_sender.cpp',
          dependencies: [dbus_dep])

executable('simple-receiver',
          'simple_receiver.cpp',
          dependencies: [dbus_dep])

dbus_cpp_connect_dep = declare_dependency(
  include_directories: incdir,
  dependencies: [dbus_dep]
)