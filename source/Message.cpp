#include <Message.h>

dashbus::Message::Message(const char *service, const char *path, const char *interface,
                          const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
}

dashbus::Message dashbus::Message::createByPointer(DBusMessage *message) {
  Message msg;

  msg.mMessage = dbus_message_copy(message);

  return msg;
}

dashbus::Message::~Message() {
  dbus_message_unref(mMessage);
}

DBusMessage *dashbus::Message::get() const {
  return mMessage;
}

dashbus::Message::operator const DBusMessage *() const {
  return mMessage;
}

dashbus::Message::operator DBusMessage *() {
  return mMessage;
}

dashbus::Message::Message() {
}
