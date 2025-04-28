#pragma once

#include <dbus/dbus.h>

#include <thread>

#include <Error.h>
#include <Message.h>
#include <ServerHandler.h>

namespace dashbus {
enum class DBusNameFlag {
  ALLOW_REPLACEMENT = DBUS_NAME_FLAG_ALLOW_REPLACEMENT,
  REPLACE_EXISTING = DBUS_NAME_FLAG_REPLACE_EXISTING,
  DO_NOT_QUEUE = DBUS_NAME_FLAG_DO_NOT_QUEUE,
};

class Connection {
public:
  Connection(DBusBusType busType);
  ~Connection();

  static Connection createByPointer(DBusConnection *connection);

  void requestName(const char *name, DBusNameFlag flags);
  void registerObjectPath(const char *path, ServerHandler &serverHandler);
  void readWriteDispatch(int timeout);
  std::thread workProcess(int timeout);

  void sendMessage(Message &message);
  Message sendMessage(Message &message, int timeout);

  DBusConnection *get() const;

private:
  Connection();

private:
  DBusConnection *mConnection;
  Error mError;
};
} // namespace dashbus