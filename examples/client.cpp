#include <dbus/dbus.h>
#include <iostream>

#include <Error.h>
#include <Message.h>

int main() {
  DBusConnection *conn;
  dashbus::Error err;
  int number = 42;
  int result;

  conn = dbus_bus_get(DBUS_BUS_SESSION, err);
  if (err.isSet()) {
    return 1;
  }

  dashbus::Message msg("com.example.MyService", "/com/example/MyObject", "com.example.MyInterface",
                       "ProcessNumber");

  if (msg.get() == NULL) {
    return 1;
  }

  DBusMessageIter writeIter;
  dbus_message_iter_init_append(msg.get(), &writeIter);
  dashbus::Message::addMessageArgument(&writeIter, number);

  DBusMessage *reply = dbus_connection_send_with_reply_and_block(conn, msg.get(), -1, err);

  if (err.isSet()) {
    return 1;
  }

  auto replyObj = dashbus::Message::createByPointer(reply);

  DBusMessageIter iter;
  dbus_message_iter_init(replyObj.get(), &iter);
  replyObj.getMessageArgument(&iter, result);

  std::cout << "Received result: " << result << "\n";

  dbus_message_unref(reply);
  dbus_connection_unref(conn);

  return 0;
}