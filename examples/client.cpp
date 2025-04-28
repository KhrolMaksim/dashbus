#include <dbus/dbus.h>
#include <iostream>

#include <Connection.h>
#include <Error.h>
#include <Message.h>

int main() {
  dashbus::Connection conn(DBUS_BUS_SESSION);
  dashbus::Message msg("com.example.MyService", "/com/example/MyObject", "com.example.MyInterface",
                       "ProcessNumber");

  int number = 42;
  msg.addArgument(number);

  auto replyObj = conn.sendMessage(msg, -1);

  int result;
  replyObj.getArgument(result);

  std::cout << "Received result: " << result << "\n";

  return 0;
}