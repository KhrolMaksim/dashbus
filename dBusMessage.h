#pragma once

#include "dBusConnection.h"
#include <dbus/dbus.h>
#include <optional>
#include <tuple>
#include <vector>

class DBusCppMessageBase {
public:
  virtual ~DBusCppMessageBase();

public:
  DBusMessage *get() const;

  // append value to the message
  void append(int value);
  void append(std::string value);
  void append(bool value);
  void append(double value);

  // append vector of values to the message
  void append(std::vector<int> const &value);
  void append(std::vector<std::string> const &value);
  void append(std::vector<bool> const &value);
  void append(std::vector<double> const &value);

  // append struct to the message
  template <typename T> void append(T const &value);

  // read value from the message
  int readInt(DBusMessageIter &iter);
  std::string readString(DBusMessageIter &iter);
  bool readBool(DBusMessageIter &iter);
  double readDouble(DBusMessageIter &iter);

  // read value from the message
  template <class T> T read(DBusMessageIter &iter);

  // read array from the message
  template <typename T> std::vector<T> readArray(DBusMessageIter &iter);

  // read struct from the message
  template <class T> T readStruct(DBusMessageIter &iter);

protected:
  DBusMessageIter mArgs;
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
