project(
  'dbus-cpp-connect',
  'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'],
)

# Исходные файлы
common_sources = files()

subdir('source')

server_sources = files(
  'examples/server.cpp',
)

client_sources = files(
  'examples/client.cpp',
)

# Собираем библиотеку
dash_bus_lib = library(
  'dash_bus',
  common_sources,
  include_directories: include_directories('include'),
  dependencies: [dependency('dbus-1')],
  # install: true,
)

dbus_dep = dependency('dbus-1')
incdir = include_directories('include')
doctest = dependency('doctest')

subdir('tests')

# Исполняемые файлы
executable(
  'dbus-server',
  common_sources + server_sources,
  include_directories: incdir,
  dependencies: [dbus_dep],
)

executable(
  'dbus-client',
  common_sources + client_sources,
  include_directories: incdir,
  dependencies: [dbus_dep],
)

executable('simple-sender', 'simple_sender.cpp', dependencies: [dbus_dep])

executable('simple-receiver', 'simple_receiver.cpp', dependencies: [dbus_dep])

dash_bus_dep = declare_dependency(
  include_directories: incdir,
  link_with: dash_bus_lib,
  dependencies: [dbus_dep]
)