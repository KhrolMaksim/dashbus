#pragma once

#include <string>

#include <dbus/dbus.h>

class DBusCppConnection {
public:
  DBusCppConnection(DBusBusType connectionType = DBUS_BUS_SESSION);
  ~DBusCppConnection();

  DBusCppConnection(const DBusCppConnection &);
  DBusCppConnection &operator=(const DBusCppConnection &);

  DBusConnection *get() const;

protected:
  DBusConnection *mConnection;
  DBusError mError;
};
