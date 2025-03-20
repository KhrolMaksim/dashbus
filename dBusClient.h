#pragma once

#include "dBusConnection.h"
#include "dBusMessage.h"

class DBusCppClient {
public:
  DBusCppClient(std::string name, std::string path, DBusBusType type = DBUS_BUS_SESSION);
  ~DBusCppClient();

  void sendMessage();

protected:
  DBusCppConnection mConnection;
  DBusError mError;
  std::string mName;
  std::string mPath;
};
