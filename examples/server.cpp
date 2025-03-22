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

int main() {
  MethodHandlers handlers;
  handlers["ProcessNumber"] = processNumberHandler;
  DBusCppServerHandler serverHandler;
  serverHandler.addInterface("com.example.MyInterface", handlers);

  // или так
  // DBusCppServerHandler serverHandler;
  // serverHandler.addMethod("com.example.MyInterface", "ProcessNumber", processNumberHandler);

  try {
    DBusCppConnection conn(DBUS_BUS_SESSION);
    conn.requestName("com.example.MyService", DBusNameFlag::REPLACE_EXISTING);
    conn.registerObjectPath("/com/example/MyObject", serverHandler);

    std::cout << "Server is running... (Press Ctrl+C to exit)" << std::endl;

    auto thread = conn.workProcess(1000);
    thread.join();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
