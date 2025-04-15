#include <Message.h>

#include <stdexcept>
#include <type_traits>

DashBus::Message::Message(const char *service, const char *path, const char *interface,
                          const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
}

DashBus::Message DashBus::Message::createByPointer(DBusMessage *message) {
  Message msg;

  msg.mMessage = dbus_message_copy(message);

  return msg;
}

template <typename T> void DashBus::Message::appendArgument(T value) {
  if constexpr (std::is_same_v<T, int>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);
  } else if constexpr (std::is_same_v<T, std::string>) {
    dbus_message_append_args(mMessage, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
  } else {
    throw NameRequestException("Unsupported argument type");
  }
}

template <typename T> T DashBus::Message::getArgument() const {
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

DashBus::Message::~Message() {
  dbus_message_unref(mMessage);
}

DBusMessage *DashBus::Message::get() const {
  return mMessage;
}

DashBus::Message::operator const DBusMessage *() const {
  return mMessage;
}

DashBus::Message::operator DBusMessage *() {
  return mMessage;
}

DashBus::Message::Message() {
}

template void DashBus::Message::appendArgument<int>(int value);
template void DashBus::Message::appendArgument<std::string>(std::string value);

template int DashBus::Message::getArgument<int>() const;
template std::string DashBus::Message::getArgument<std::string>() const;
