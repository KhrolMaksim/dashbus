#include <dbus/dbus.h>
#include <iostream>

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  DBusError err;
  int number;

  dbus_error_init(&err);

  if (dbus_message_is_method_call(msg, "com.example.MyInterface", "ProcessNumber")) {
    if (!dbus_message_get_args(msg, &err, DBUS_TYPE_INT32, &number, DBUS_TYPE_INVALID)) {
      dbus_error_free(&err);
      return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    std::cout << "Received number: " << number << "\n";

    int result = number * 2;

    DBusMessage *reply = dbus_message_new_method_return(msg);
    if (reply == NULL) {
      return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }

    if (!dbus_message_append_args(reply, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID)) {
      dbus_message_unref(reply);
      return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }

    if (!dbus_connection_send(conn, reply, NULL)) {
      dbus_message_unref(reply);
      return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }

    dbus_connection_flush(conn);

    dbus_message_unref(reply);
    return DBUS_HANDLER_RESULT_HANDLED;
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main() {
  DBusConnection *conn;
  DBusError err;

  dbus_error_init(&err);

  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Connection Error: " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }

  int ret =
      dbus_bus_request_name(conn, "com.example.MyService", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Name Error: " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    std::cerr << "Not Primary Owner" << std::endl;
    return 1;
  }

  DBusObjectPathVTable vtable = {NULL, &handle_message, NULL, NULL, NULL, NULL};
  if (!dbus_connection_register_object_path(conn, "/com/example/MyObject", &vtable, NULL)) {
    std::cerr << "Cannot register object path" << std::endl;
    return 1;
  }

  std::cout << "Server is running... (Press Ctrl+C to exit)" << std::endl;

  while (true) {
    dbus_connection_read_write_dispatch(conn, 1000); // 1000 мс
    std::cout << "Waiting for messages..." << std::endl;
  }

  dbus_connection_unref(conn);

  return 0;
}
