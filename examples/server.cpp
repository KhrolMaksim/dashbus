#include <iostream>

#include <Connection.h>
#include <Error.h>
#include <Message.h>
#include <ServerHandler.h>

DBusHandlerResult processNumberHandler(dashbus::Connection &connection, dashbus::Message &message) {
  int number;

  try {
    message.getArgument(number);
  } catch (const dashbus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  std::cout << "Received number: " << number << "\n";

  int result = number * 2;

  dashbus::Message reply = message.getReturnMessage();

  if (reply == NULL) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  try {
    reply.addArgument(result);
  } catch (const dashbus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  try {
    connection.sendMessage(reply);
  } catch (const dashbus::NameRequestException &e) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  return DBUS_HANDLER_RESULT_HANDLED;
}

int main() {
  dashbus::MethodHandlers handlers;
  handlers["ProcessNumber"] = processNumberHandler;
  dashbus::ServerHandler serverHandler;
  serverHandler.addInterface("com.example.MyInterface", handlers);

  // или так
  // ServerHandler serverHandler;
  // serverHandler.addMethod("com.example.MyInterface", "ProcessNumber", processNumberHandler);

  try {
    dashbus::Connection conn(DBUS_BUS_SESSION);
    conn.requestName("com.example.MyService", dashbus::DBusNameFlag::REPLACE_EXISTING);
    conn.registerObjectPath("/com/example/MyObject", serverHandler);

    std::cout << "Server is running... (Press Ctrl+C to exit)" << std::endl;

    auto thread = conn.workProcess(1000);
    thread.join();
  } catch (const dashbus::NameRequestException &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
