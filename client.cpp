#include <dbus/dbus.h>
#include <iostream>

#include "dBusMessage.h"

int main() {
  DBusCppMessage msg("com.example.MyService", "/com/example/MyObject", "com.example.MyInterface",
                     "ProcessNumber");
  DBusCppConnection conn;

  int number = 42;
  int result;

  // msg.append(number);
  dbus_message_append_args(msg.get(), DBUS_TYPE_INT32, &number, DBUS_TYPE_INVALID);

  DBusCppReplyMessage reply(conn, msg);

  if (!dbus_message_get_args(reply.get(), NULL, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID)) {
    return 1;
  }

  std::cout << "Received result: " << result << "\n";

  return 0;
}