project('dbus-cpp-connect', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

# Исходные файлы
common_sources = files([
  'dBusConnection.cpp',
  'dBusMessage.cpp',
  'dBusClient.cpp',
  'dBusServer.cpp',
])

server_sources = files([
  'server.cpp',
])

client_sources = files([
  'client.cpp',
])

dbus_dep = dependency('dbus-1')

# Исполняемые файлы
executable('dbus-server', 
          common_sources + server_sources, 
          dependencies: [dbus_dep])

executable('dbus-client', 
          common_sources + client_sources, 
          dependencies: [dbus_dep])
