tests_source = files(
    'Connection.test.cpp',
    'Error.test.cpp',
    'Message.test.cpp',
    'ServerHandler.test.cpp',
    'test.cpp'
)

test('connect_test', executable('test_connect', common_sources + 'Connection.test.cpp', include_directories: incdir, dependencies: [doctest, dbus_dep]))
test('error_test', executable('test_error', common_sources + 'Error.test.cpp', include_directories: incdir, dependencies: [doctest, dbus_dep]))
test('message_test', executable('test_message', common_sources + 'Message.test.cpp', include_directories: incdir, dependencies: [doctest, dbus_dep]))
test('server_handler_test', executable('test_server_handler', common_sources + 'ServerHandler.test.cpp', include_directories: incdir, dependencies: [doctest, dbus_dep]))