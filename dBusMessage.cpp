#include "dBusMessage.h"

#include <iostream>

DBusMessage *DBusCppMessageBase::get() const {
  return mMessage;
}

void DBusCppMessageBase::append(int value) {
  dbus_message_iter_append_basic(&mArgs, DBUS_TYPE_INT32, &value);
}

void DBusCppMessageBase::append(std::string value) {
  dbus_message_iter_append_basic(&mArgs, DBUS_TYPE_STRING, &value);
}

void DBusCppMessageBase::append(bool value) {
  dbus_message_iter_append_basic(&mArgs, DBUS_TYPE_BOOLEAN, &value);
}

void DBusCppMessageBase::append(double value) {
  dbus_message_iter_append_basic(&mArgs, DBUS_TYPE_DOUBLE, &value);
}

void DBusCppMessageBase::append(std::vector<int> const &value) {
  DBusMessageIter arrayIter;
  dbus_message_iter_open_container(&mArgs, DBUS_TYPE_ARRAY, DBUS_TYPE_INT32_AS_STRING, &arrayIter);

  for (auto const &v : value) {
    append(v);
  }

  dbus_message_iter_close_container(&mArgs, &arrayIter);
}

void DBusCppMessageBase::append(std::vector<std::string> const &value) {
  DBusMessageIter arrayIter;
  dbus_message_iter_open_container(&mArgs, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &arrayIter);

  for (auto const &v : value) {
    append(v);
  }

  dbus_message_iter_close_container(&mArgs, &arrayIter);
}

void DBusCppMessageBase::append(std::vector<bool> const &value) {
  DBusMessageIter arrayIter;
  dbus_message_iter_open_container(&mArgs, DBUS_TYPE_ARRAY, DBUS_TYPE_BOOLEAN_AS_STRING,
                                   &arrayIter);

  for (auto const &v : value) {
    append(v);
  }

  dbus_message_iter_close_container(&mArgs, &arrayIter);
}

void DBusCppMessageBase::append(std::vector<double> const &value) {
  DBusMessageIter arrayIter;
  dbus_message_iter_open_container(&mArgs, DBUS_TYPE_ARRAY, DBUS_TYPE_DOUBLE_AS_STRING, &arrayIter);

  for (auto const &v : value) {
    append(v);
  }

  dbus_message_iter_close_container(&mArgs, &arrayIter);
}

template <> void DBusCppMessageBase::append<std::optional<int>>(std::optional<int> const &value) {
  if (value) {
    append(*value);
  }
}

template <>
void DBusCppMessageBase::append<std::optional<std::string>>(
    std::optional<std::string> const &value) {
  if (value) {
    append(*value);
  }
}

template <> void DBusCppMessageBase::append<std::optional<bool>>(std::optional<bool> const &value) {
  if (value) {
    append(*value);
  }
}

template <>
void DBusCppMessageBase::append<std::optional<double>>(std::optional<double> const &value) {
  if (value) {
    append(*value);
  }
}

template <class T> void DBusCppMessageBase::append(T const &value) {
  DBusMessageIter structIter;
  dbus_message_iter_open_container(&mArgs, DBUS_TYPE_STRUCT, NULL, &structIter);

  std::apply([this](auto &&...args) { ((this->append(args)), ...); }, value.getTuple());

  dbus_message_iter_close_container(&mArgs, &structIter);
}

int DBusCppMessageBase::readInt(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INT32) {
    throw std::runtime_error("Expected int argument");
  }

  int value;
  dbus_message_iter_get_basic(&iter, &value);
  dbus_message_iter_next(&iter);
  return value;
}

std::string DBusCppMessageBase::readString(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_STRING) {
    throw std::runtime_error("Expected string argument");
  }

  const char *value;
  dbus_message_iter_get_basic(&iter, &value);
  dbus_message_iter_next(&iter);
  return std::string(value);
}

bool DBusCppMessageBase::readBool(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_BOOLEAN) {
    throw std::runtime_error("Expected bool argument");
  }

  bool value;
  dbus_message_iter_get_basic(&iter, &value);
  dbus_message_iter_next(&iter);
  return value;
}

double DBusCppMessageBase::readDouble(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_DOUBLE) {
    throw std::runtime_error("Expected double argument");
  }

  double value;
  dbus_message_iter_get_basic(&iter, &value);
  dbus_message_iter_next(&iter);
  return value;
}

template <class T> T DBusCppMessageBase::read(DBusMessageIter &iter) {
  if constexpr (std::is_same_v<T, int>) {
    return readInt(iter);
  } else if constexpr (std::is_same_v<T, std::string>) {
    return readString(iter);
  } else if constexpr (std::is_same_v<T, bool>) {
    return readBool(iter);
  } else if constexpr (std::is_same_v<T, double>) {
    return readDouble(iter);
  } else {
    static_assert(true, "Unsupported type");
  }
}

template <class T> std::vector<T> DBusCppMessageBase::readArray(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_ARRAY) {
    throw std::runtime_error("Expected array argument");
  }

  std::vector<T> value;
  DBusMessageIter arrayIter;
  dbus_message_iter_recurse(&iter, &arrayIter);

  while (dbus_message_iter_get_arg_type(&arrayIter) != DBUS_TYPE_INVALID) {
    value.push_back(read<T>(arrayIter));
  }

  return value;
}

template <class T> T DBusCppMessageBase::readStruct(DBusMessageIter &iter) {
  static_assert(true, "Unsupported struct type");
}

struct Struct {
  std::string name;
  int age;
  bool is_male;
  double height;

  std::tuple<std::string, int, bool, double> getTuple() const {
    return std::make_tuple(name, age, is_male, height);
  }
};

template <> Struct DBusCppMessageBase::readStruct<Struct>(DBusMessageIter &iter) {
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_STRUCT) {
    throw std::runtime_error("Expected struct argument");
  }

  DBusMessageIter structIter;
  dbus_message_iter_recurse(&iter, &structIter);

  Struct value;

  value.name = read<std::string>(structIter);
  value.age = read<int>(structIter);
  value.is_male = read<bool>(structIter);
  value.height = read<double>(structIter);

  return value;
}

DBusCppMessageBase::~DBusCppMessageBase() {
  dbus_error_free(&mError);

  if (mMessage) {
    dbus_message_unref(mMessage);
  }
}

DBusCppMessage::DBusCppMessage(std::string name, std::string path, std::string interface,
                               std::string method) {
  dbus_error_init(&mError);

  mMessage =
      dbus_message_new_method_call(name.c_str(), path.c_str(), interface.c_str(), method.c_str());

  if (not mMessage) {
    std::cerr << "Failed to create message" << std::endl;
    return;
  }
}

DBusCppReplyMessage::DBusCppReplyMessage(DBusCppConnection &connection, DBusCppMessage &message,
                                         int timeout) {
  dbus_error_init(&mError);

  mMessage =
      dbus_connection_send_with_reply_and_block(connection.get(), message.get(), timeout, &mError);

  if (not mMessage) {
    std::cerr << "Failed to create message" << std::endl;
    return;
  }
}

DBusCppSignalMessage::DBusCppSignalMessage(std::string name, std::string path,
                                           std::string interface, std::string signal) {
  dbus_error_init(&mError);

  mMessage = dbus_message_new_signal(path.c_str(), interface.c_str(), signal.c_str());
}

template void DBusCppMessageBase::append<Struct>(Struct const &value);

template void DBusCppMessageBase::append<std::optional<int>>(std::optional<int> const &value);
template void
DBusCppMessageBase::append<std::optional<std::string>>(std::optional<std::string> const &value);
template void DBusCppMessageBase::append<std::optional<bool>>(std::optional<bool> const &value);
template void DBusCppMessageBase::append<std::optional<double>>(std::optional<double> const &value);

template std::vector<int> DBusCppMessageBase::readArray<int>(DBusMessageIter &iter);
template std::vector<std::string> DBusCppMessageBase::readArray<std::string>(DBusMessageIter &iter);
template std::vector<bool> DBusCppMessageBase::readArray<bool>(DBusMessageIter &iter);
template std::vector<double> DBusCppMessageBase::readArray<double>(DBusMessageIter &iter);

template int DBusCppMessageBase::read<int>(DBusMessageIter &iter);
template std::string DBusCppMessageBase::read<std::string>(DBusMessageIter &iter);
template bool DBusCppMessageBase::read<bool>(DBusMessageIter &iter);
template double DBusCppMessageBase::read<double>(DBusMessageIter &iter);
