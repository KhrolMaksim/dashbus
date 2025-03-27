#include <dBusCppMessage.h>

#include <stdexcept>
#include <type_traits>

DBusCppMessage::DBusCppMessage(const char *service, const char *path, const char *interface,
                               const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
}

DBusCppMessage DBusCppMessage::createByPointer(DBusMessage *message) {
  DBusCppMessage msg;

  msg.mMessage = dbus_message_copy(message);

  return msg;
}

template <typename T> void DBusCppMessage::appendArgument(T value) {
  if constexpr (std::is_same_v<T, int>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);
  } else if constexpr (std::is_same_v<T, std::string>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
  } else {
    throw std::runtime_error("Unsupported argument type");
  }
}

template <typename T> T DBusCppMessage::getArgument() const {
  T value;

  if constexpr (std::is_same_v<T, int>) {
    dbus_message_get_args(mMessage, NULL, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);
  } else if constexpr (std::is_same_v<T, std::string>) {
    dbus_message_get_args(mMessage, NULL, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
  } else {
    throw std::runtime_error("Unsupported argument type");
  }

  return value;
}

DBusCppMessage::~DBusCppMessage() {
  dbus_message_unref(mMessage);
}

DBusMessage *DBusCppMessage::get() const {
  return mMessage;
}

DBusCppMessage::operator const DBusMessage *() const {
  return mMessage;
}

DBusCppMessage::operator DBusMessage *() {
  return mMessage;
}

DBusCppMessage::DBusCppMessage() {
}

template void DBusCppMessage::appendArgument<int>(int value);
template void DBusCppMessage::appendArgument<std::string>(std::string value);

template int DBusCppMessage::getArgument<int>() const;
template std::string DBusCppMessage::getArgument<std::string>() const;
