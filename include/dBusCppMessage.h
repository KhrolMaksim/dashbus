#pragma once

#include <dbus/dbus.h>

#include <dBusCppException.h>

class DBusCppMessage {
public:
  DBusCppMessage(const char *service, const char *path, const char *interface, const char *method);
  ~DBusCppMessage();

  static DBusCppMessage createByPointer(DBusMessage *message);

  template <typename T> void appendArgument(T value);
  template <typename T> T getArgument() const;

  DBusMessage *get() const;
  operator const DBusMessage *() const;
  operator DBusMessage *();

private:
  DBusCppMessage();

private:
  DBusMessage *mMessage;
};