#include <dbus/dbus.h>
#include <iostream>
#include <map>

#include <dBusCppConnection.h>
#include <dBusCppError.h>
#include <dBusCppServerHandler.h>

DBusHandlerResult processNumberHandler(DBusConnection *conn, DBusMessage *msg) {
  DBusCppError err;
  int number;

  if (!dbus_message_get_args(msg, err, DBUS_TYPE_INT32, &number, DBUS_TYPE_INVALID)) {
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

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  DBusCppServerHandler *serverHandler = static_cast<DBusCppServerHandler *>(user_data);

  const char *iface = dbus_message_get_interface(msg);
  if (not serverHandler->hasInterface(iface)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  const char *member = dbus_message_get_member(msg);
  if (not serverHandler->hasMethod(iface, member)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  MethodHandler handler = serverHandler->getMethodHandler(iface, member);
  return handler(conn, msg);
}

int main() {
  DBusCppConnection conn(DBUS_BUS_SESSION);
  DBusCppError err;

  int ret = dbus_bus_request_name(conn.get(), "com.example.MyService",
                                  DBUS_NAME_FLAG_REPLACE_EXISTING, err);
  if (err.isSet()) {
    std::cerr << "Name Error: " << err.getMessage() << std::endl;
    return 1;
  }
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    std::cerr << "Not Primary Owner" << std::endl;
    return 1;
  }

  DBusCppServerHandler serverHandler;
  serverHandler.addMethod("com.example.MyInterface", "ProcessNumber", processNumberHandler);
  serverHandler.addMethod("com.example.MyInterface", "ProcessNumber1", processNumberHandler);
  serverHandler.addMethod("com.example.MyInterface", "ProcessNumber2", processNumberHandler);
  serverHandler.addMethod("com.example.MyInterface", "ProcessNumber3", processNumberHandler);

  DBusObjectPathVTable vtable = {NULL, &handle_message, NULL, NULL, NULL, NULL};
  if (!dbus_connection_register_object_path(conn.get(), "/com/example/MyObject", &vtable,
                                            &serverHandler)) {
    std::cerr << "Cannot register object path" << std::endl;
    return 1;
  }

  std::cout << "Server is running... (Press Ctrl+C to exit)" << std::endl;

  while (true) {
    dbus_connection_read_write_dispatch(conn.get(), 1000); // 1000 мс
    std::cout << "Waiting for messages..." << std::endl;
  }

  return 0;
}
