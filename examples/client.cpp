#include <dbus/dbus.h>
#include <iostream>

#include <Error.h>

int main() {
  DBusConnection *conn;
  dashbus::Error err;
  DBusMessage *msg, *reply;
  int number = 42;
  int result;

  conn = dbus_bus_get(DBUS_BUS_SESSION, err);
  if (err.isSet()) {
    return 1;
  }

  msg = dbus_message_new_method_call("com.example.MyService", "/com/example/MyObject",
                                     "com.example.MyInterface", "ProcessNumber");

  if (msg == NULL) {
    return 1;
  }

  if (!dbus_message_append_args(msg, DBUS_TYPE_INT32, &number, DBUS_TYPE_INVALID)) {
    return 1;
  }

  reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, err);

  if (err.isSet()) {
    return 1;
  }

  if (!dbus_message_get_args(reply, err, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID)) {
    return 1;
  }

  std::cout << "Received result: " << result << "\n";

  dbus_message_unref(msg);
  dbus_message_unref(reply);
  dbus_connection_unref(conn);

  return 0;
}