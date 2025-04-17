#pragma once

#include <dbus/dbus.h>

#include <Concepts.h>
#include <Exception.h>

/**
 * @todo Временная структура для тестов
 * @todo При возможности избавиться от введения пользовательских структур в файл(
    уже есть вариант
 **/

class A {
public:
  auto getFieldsRef() {
    return std::tie(num, fNum, name, flag1, flag2);
  }
  auto getFieldsRef() const {
    return std::tie(num, fNum, name, flag1, flag2);
  }

  int num = 0;
  double fNum = 0.0;
  std::string name;
  bool flag1 = false;
  bool flag2 = false;
};

namespace dashbus {
class Message {
public:
  Message(const char *service, const char *path, const char *interface, const char *method);
  ~Message();

  static Message createByPointer(DBusMessage *message);

  DBusMessage *get() const;
  operator const DBusMessage *() const;
  operator DBusMessage *();

  /// @todo сделать private
  static void addMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto value);
  /// @todo сделать private
  static void addMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto value);
  /// @todo сделать private
  static void addMessageArgument(DBusMessageIter *msgIter, StructDBusType auto value);

  /// @todo сделать private
  static void getMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto &value);
  /// @todo сделать private
  static void getMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto &value);
  /// @todo сделать private
  static void getMessageArgument(DBusMessageIter *msgIter, StructDBusType auto &value);

private:
  Message();

private:
  DBusMessage *mMessage;
};
} // namespace dashbus