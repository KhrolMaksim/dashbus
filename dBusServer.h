#pragma once

#include "dBusConnection.h"

class DBusCppServer {
public:
  DBusCppServer(std::string name, DBusBusType type = DBUS_BUS_SESSION);
  DBusCppServer(std::string name, int flags, DBusBusType type = DBUS_BUS_SESSION);
  ~DBusCppServer();

  void run();

protected:
  DBusCppConnection mConnection;
  DBusError mError;
};
