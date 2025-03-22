#pragma once

#include <dbus/dbus.h>

#include <dBusCppError.h>

class DBusCppConnection {
public:
  DBusCppConnection(DBusBusType busType);
  ~DBusCppConnection();

  DBusConnection *get() const;

private:
  DBusConnection *mConnection;
  DBusCppError mError;
};