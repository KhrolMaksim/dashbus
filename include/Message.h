#pragma once

#include <dbus/dbus.h>

#include <Concepts.h>
#include <Exception.h>

#include "Detail.h"

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

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto value) {
  using CurrentType = std::decay_t<decltype(value)>;

  if constexpr (Byte<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BYTE, &value);
  } else if constexpr (DBusBoolean<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BOOLEAN, &value);
  } else if constexpr (CppBoolean<CurrentType>) {
    dbus_bool_t temp = value ? TRUE : FALSE;
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BOOLEAN, &temp);
  } else if constexpr (SInt16<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT16, &value);
  } else if constexpr (UInt16<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT16, &value);
  } else if constexpr (SInt32<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT32, &value);
  } else if constexpr (UInt32<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT32, &value);
  } else if constexpr (SInt64<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT64, &value);
  } else if constexpr (UInt64<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT64, &value);
  } else if constexpr (Double<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_DOUBLE, &value);
  } else if constexpr (String<CurrentType>) {
    const char *c_str = value.c_str();
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_STRING, &c_str);
  } else {
    static_assert(false, "Выполнен вызов addMessageArgument для необработанного BaseDBusType");
  }
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto value) {
  using CurrentType = std::decay_t<decltype(value)>;
  if constexpr (Array<CurrentType>) {
    using ElementType = typename CurrentType::value_type;
    ElementType fictiveElement;

    std::string elem_sig = detail::getDBusSignature(fictiveElement);
    const char *elem_sig_c_str = elem_sig.c_str();

    DBusMessageIter arrIter;
    dbus_message_iter_open_container(msgIter, DBUS_TYPE_ARRAY, elem_sig_c_str, &arrIter);

    for (auto const &elem : value) {
      Message::addMessageArgument(&arrIter, elem);
    }

    dbus_message_iter_close_container(msgIter, &arrIter);
  } else {
    static_assert(false, "Выполнен вызов addMessageArgument для необработанного CompoundDBusType");
  }
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, StructDBusType auto value) {
  DBusMessageIter structIter;
  dbus_message_iter_open_container(msgIter, DBUS_TYPE_STRUCT, NULL, &structIter);

  auto addMessageArgumentTuple = [&structIter](auto &&...args) {
    (Message::addMessageArgument(&structIter, args), ...);
  };

  auto fieldRefs = value.getFieldsRef();
  std::apply(addMessageArgumentTuple, fieldRefs);

  dbus_message_iter_close_container(msgIter, &structIter);
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto &value) {
  using CurrentType = std::decay_t<decltype(value)>;

  if constexpr (Byte<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (DBusBoolean<CurrentType> or CppBoolean<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt16<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt16<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt32<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt32<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt64<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt64<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (Double<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (String<CurrentType>) {
    const char *tempValue;
    dbus_message_iter_get_basic(msgIter, &tempValue);
    value = std::string(tempValue);
  } else {
    static_assert(false, "Выполнен вызов getMessageArgument для необработанного BaseDBusType");
  }

  dbus_message_iter_next(msgIter);
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto &value) {
  using Type = std::decay_t<decltype(value)>;
  if constexpr (Array<Type>) {
    using ElementType = typename Type::value_type;

    DBusMessageIter subIter;
    dbus_message_iter_recurse(msgIter, &subIter);

    value.clear();

    while (dbus_message_iter_get_arg_type(&subIter) != DBUS_TYPE_INVALID) {
      ElementType elem;
      Message::getMessageArgument(&subIter, elem);
      value.push_back(std::move(elem));
      // Сдвигом после элементов возложено на сами элементы
      // dbus_message_iter_next(&subIter);
    }

    dbus_message_iter_next(msgIter);
  } else {
    static_assert(false, "Unsupported CompoundDBusType");
  }
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, StructDBusType auto &value) {
  DBusMessageIter structIter;
  dbus_message_iter_recurse(msgIter, &structIter);

  auto addMessageArgumentTuple = [&structIter](auto &&...args) {
    (Message::getMessageArgument(&structIter, args), ...);
  };

  auto fieldRefs = value.getFieldsRef();
  std::apply(addMessageArgumentTuple, fieldRefs);

  dbus_message_iter_next(msgIter);
}