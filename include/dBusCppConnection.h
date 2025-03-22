#pragma once

#include <dbus/dbus.h>

#include <thread>

#include <dBusCppError.h>
#include <dBusCppServerHandler.h>

enum class DBusNameFlag {
  ALLOW_REPLACEMENT = DBUS_NAME_FLAG_ALLOW_REPLACEMENT,
  REPLACE_EXISTING = DBUS_NAME_FLAG_REPLACE_EXISTING,
  DO_NOT_QUEUE = DBUS_NAME_FLAG_DO_NOT_QUEUE,
};

class DBusCppConnection {
public:
  DBusCppConnection(DBusBusType busType);
  ~DBusCppConnection();

  void requestName(const char *name, DBusNameFlag flags);
  void registerObjectPath(const char *path, DBusCppServerHandler &serverHandler);
  void readWriteDispatch(int timeout);
  std::thread workProcess(int timeout);

  DBusConnection *get() const;

private:
  DBusConnection *mConnection;
  DBusCppError mError;
};