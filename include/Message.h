#pragma once

#include <dbus/dbus.h>

#include <Exception.h>

namespace dashbus {
class Message {
public:
  Message(const char *service, const char *path, const char *interface, const char *method);
  ~Message();

  static Message createByPointer(DBusMessage *message);

  template <typename T> void appendArgument(T value);
  template <typename T> T getArgument() const;

  DBusMessage *get() const;
  operator const DBusMessage *() const;
  operator DBusMessage *();

private:
  Message();

private:
  DBusMessage *mMessage;
};
} // namespace dashbus