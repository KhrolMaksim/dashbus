#include <iostream>

#include <dBusCppConnection.h>
#include <dBusCppError.h>
#include <dBusCppMessage.h>
#include <dBusCppServerHandler.h>

DBusHandlerResult processNumberHandler(DBusConnection *conn, DBusMessage *msg) {
  DBusCppError err;
  int number;

  DBusCppMessage incoming = DBusCppMessage::createByPointer(msg);

  try {
    number = incoming.getArgument<int>();
  } catch (const std::runtime_error &e) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  std::cout << "Received number: " << number << "\n";

  int result = number * 2;

  DBusCppMessage reply = DBusCppMessage::createByPointer(dbus_message_new_method_return(msg));

  if (reply == NULL) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  try {
    reply.appendArgument(result);
  } catch (const std::runtime_error &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  DBusCppConnection connection = DBusCppConnection::createByPointer(conn);

  try {
    connection.sendMessage(reply);
  } catch (const std::runtime_error &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

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
