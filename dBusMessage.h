#pragma once

#include "dBusConnection.h"
#include <dbus/dbus.h>

class DBusCppMessageBase {
public:
  virtual ~DBusCppMessageBase();

protected:
  DBusMessage *get() const;

protected:
  DBusMessage *mMessage;
  DBusError mError;
};

class DBusCppReplyMessage;

class DBusCppMessage : public DBusCppMessageBase {
public:
  DBusCppMessage(std::string name, std::string path, std::string interface, std::string method);
  ~DBusCppMessage() = default;

private:
  friend class DBusCppReplyMessage;
};

class DBusCppReplyMessage : public DBusCppMessageBase {
public:
  DBusCppReplyMessage(DBusCppConnection &connection, DBusCppMessage &message, int timeout = -1);
  ~DBusCppReplyMessage() = default;
};

class DBusCppSignalMessage : public DBusCppMessageBase {
public:
  DBusCppSignalMessage(std::string name, std::string path, std::string interface,
                       std::string signal);
  ~DBusCppSignalMessage() = default;
};
