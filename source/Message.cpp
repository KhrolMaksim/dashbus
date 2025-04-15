#include <Message.h>

#include <stdexcept>
#include <type_traits>

dashbus::Message::Message(const char *service, const char *path, const char *interface,
                          const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
}

dashbus::Message dashbus::Message::createByPointer(DBusMessage *message) {
  Message msg;

  msg.mMessage = dbus_message_copy(message);

  return msg;
}

template <typename T> void dashbus::Message::appendArgument(T value) {
  if constexpr (std::is_same_v<T, int>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);
  } else if constexpr (std::is_same_v<T, std::string>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
  } else {
    throw NameRequestException("Unsupported argument type");
  }
}

template <typename T> T dashbus::Message::getArgument() const {
  T value;

  if constexpr (std::is_same_v<T, int>) {
    dbus_message_get_args(mMessage, NULL, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);
  } else if constexpr (std::is_same_v<T, std::string>) {
    dbus_message_get_args(mMessage, NULL, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
  } else {
    throw NameRequestException("Unsupported argument type");
  }

  return value;
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

template void dashbus::Message::appendArgument<int>(int value);
template void dashbus::Message::appendArgument<std::string>(std::string value);

template int dashbus::Message::getArgument<int>() const;
template std::string dashbus::Message::getArgument<std::string>() const;
