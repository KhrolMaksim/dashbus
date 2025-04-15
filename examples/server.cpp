#include <iostream>

#include <Connection.h>
#include <Error.h>
#include <Message.h>
#include <ServerHandler.h>

DBusHandlerResult processNumberHandler(DBusConnection *conn, DBusMessage *msg) {
  int number;

  DashBus::Message incoming = DashBus::Message::createByPointer(msg);

  try {
    number = incoming.getArgument<int>();
  } catch (const DashBus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  std::cout << "Received number: " << number << "\n";

  int result = number * 2;

  DashBus::Message reply = DashBus::Message::createByPointer(dbus_message_new_method_return(msg));

  if (reply == NULL) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  try {
    reply.appendArgument(result);
  } catch (const DashBus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  DashBus::Connection connection = DashBus::Connection::createByPointer(conn);

  try {
    connection.sendMessage(reply);
  } catch (const DashBus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  return DBUS_HANDLER_RESULT_HANDLED;
}

int main() {
  DashBus::MethodHandlers handlers;
  handlers["ProcessNumber"] = processNumberHandler;
  DashBus::ServerHandler serverHandler;
  serverHandler.addInterface("com.example.MyInterface", handlers);

  // или так
  // ServerHandler serverHandler;
  // serverHandler.addMethod("com.example.MyInterface", "ProcessNumber", processNumberHandler);

  try {
    DashBus::Connection conn(DBUS_BUS_SESSION);
    conn.requestName("com.example.MyService", DashBus::DBusNameFlag::REPLACE_EXISTING);
    conn.registerObjectPath("/com/example/MyObject", serverHandler);

    std::cout << "Server is running... (Press Ctrl+C to exit)" << std::endl;

    auto thread = conn.workProcess(1000);
    thread.join();
  } catch (const DashBus::NameRequestException &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
