#pragma once

#include <dbus/dbus.h>

class DBusCppError {
public:
  DBusCppError();
  ~DBusCppError();

  DBusError *get();

private:
  DBusError mError;
};
