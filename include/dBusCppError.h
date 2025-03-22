#pragma once

#include <dbus/dbus.h>

class DBusCppError {
public:
  DBusCppError();
  ~DBusCppError();

  dbus_bool_t isSet() const;
  const char *getMessage() const;

  // const DBusError *get() const;

  operator const DBusError *() const;
  operator DBusError *();

private:
  DBusError mError;
};
