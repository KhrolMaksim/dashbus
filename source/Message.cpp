#include <Message.h>

dashbus::Message::Message(const char *service, const char *path, const char *interface,
                          const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
}

dashbus::Message dashbus::Message::createByPointer(DBusMessage *message) {
  Message msg;

  msg.mMessage = dbus_message_copy(message);

  return msg;
}

dashbus::Message::~Message() {
  dbus_message_unref(mMessage);
}

DBusMessage *dashbus::Message::get() const {
  return mMessage;
}

dashbus::Message::operator const DBusMessage *() const {
  return mMessage;
}

dashbus::Message::operator DBusMessage *() {
  return mMessage;
}

dashbus::Message::Message() {
}

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
  using CurrentType = decltype(value);
  if constexpr (Array<CurrentType>) {
    DBusMessageIter arrIter;
    dbus_message_iter_open_container(msgIter, DBUS_TYPE_ARRAY, NULL, &arrIter);
    for (auto const &elem : value) {
      Message::addMessageArgument(msgIter, value);
    }
  } else {
    static_assert(false, "Выполнен вызов addMessageArgument для необработанного CompoundDBusType");
  }
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, StructDBusType auto value) {
  // using CurrentType = decltype(value);
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
  using Type = decltype(value);
  using ElementType = typename Type::value_type;
  DBusMessageIter subIter;
  dbus_message_iter_recurse(msgIter, &subIter);

  while (dbus_message_iter_get_arg_type(&subIter) != DBUS_TYPE_INVALID) {
    ElementType elem;
    getMessageArgument<ElementType>(&subIter, elem);
    value.push_back(elem);
    dbus_message_iter_next(&subIter);
  }
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, StructDBusType auto &value) {
  DBusMessageIter structIter;
  dbus_message_iter_recurse(msgIter, &structIter);

  auto addMessageArgumentTuple = [&structIter, &msgIter](auto &&...args) {
    (Message::getMessageArgument(&structIter, args), ...);
  };

  auto fieldRefs = value.getFieldsRef();
  std::apply(addMessageArgumentTuple, fieldRefs);

  dbus_message_iter_next(msgIter);
}

template void dashbus::Message::addMessageArgument<int>(DBusMessageIter *, int);
template void dashbus::Message::addMessageArgument<std::string>(DBusMessageIter *, std::string);
template void dashbus::Message::addMessageArgument<double>(DBusMessageIter *, double);
template void dashbus::Message::addMessageArgument<bool>(DBusMessageIter *, bool);
template void dashbus::Message::addMessageArgument<A>(DBusMessageIter *, A);

template void dashbus::Message::getMessageArgument<int>(DBusMessageIter *, int &);
template void dashbus::Message::getMessageArgument<std::string>(DBusMessageIter *, std::string &);
template void dashbus::Message::getMessageArgument<double>(DBusMessageIter *, double &);
template void dashbus::Message::getMessageArgument<bool>(DBusMessageIter *, bool &);
template void dashbus::Message::getMessageArgument<A>(DBusMessageIter *, A &);
