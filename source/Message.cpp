#include <Message.h>

namespace dashbus {

bool MessageIter::initWriteIter(Message &message) {
  if (isInit) {
    return false;
  }

  if (message.get() == NULL) {
    return false;
  }

  dbus_message_iter_init_append(message, &iter);
  isInit = true;

  return true;
}

bool MessageIter::initReadIter(Message &message) {
  if (isInit) {
    return false;
  }

  if (message.get() == NULL) {
    return false;
  }

  dbus_message_iter_init(message, &iter);
  isInit = true;

  return true;
}

MessageIter::operator bool() {
  return isInit;
}

MessageIter::operator DBusMessageIter *() {
  return &iter;
}

Message::Message(const char *service, const char *path, const char *interface, const char *method) {
  if (not service or not path or not interface or not method) {
    throw Exception("Cannot create message with null parameters");
  }

  mMessage = dbus_message_new_method_call(service, path, interface, method);
  if (not mMessage) {
    throw Exception("Failed to create method call message");
  }

  mType = Type::METHOD;
}

Message::Message(const char *path, const char *interface, const char *name) {
  if (not path or not interface or not name) {
    throw Exception("Cannot create message with null parameters");
  }

  mMessage = dbus_message_new_signal(path, interface, name);
  if (not mMessage) {
    throw Exception("Failed to create signal message");
  }

  mType = Type::SIGNAL;
}

Message Message::createByPointer(DBusMessage *message) {
  if (not message) {
    throw Exception("Cannot create Message from null pointer");
  }

  Message msg;
  msg.mMessage = dbus_message_copy(message);

  if (not msg.mMessage) {
    throw Exception("Failed to copy message");
  }

  return msg;
}

Message::~Message() {
  if (mMessage != NULL) {
    dbus_message_unref(mMessage);
  }
}

DBusMessage *Message::get() const {
  return mMessage;
}

Message::operator const DBusMessage *() const {
  return mMessage;
}

Message::operator DBusMessage *() {
  return mMessage;
}

void Message::startWriteArguments() {
  if (not mMessage) {
    throw Exception("Cannot start write arguments on null message");
  }

  mWriteIter.initWriteIter(*this);
}

void Message::startReadArguments() {
  if (not mMessage) {
    throw Exception("Cannot start read arguments on null message");
  }

  mReadIter.initReadIter(*this);
}

int Message::getArgumentType() {
  if (not mMessage) {
    throw Exception("Cannot get argument type from null message");
  }

  if (not mReadIter) {
    startReadArguments();
  }

  return dbus_message_iter_get_arg_type(mReadIter);
}

Message Message::getReturnMessage() {
  if (not mMessage) {
    throw Exception("Cannot create return message from null message");
  }

  Message message;
  message.mMessage = dbus_message_new_method_return(mMessage);
  if (not message.mMessage) {
    throw Exception("Failed to create method return message");
  }

  message.mType = Type::METHOD_REPLY;
  dbus_message_set_reply_serial(message.mMessage, dbus_message_get_serial(mMessage));

  return message;
}

dbus_uint32_t Message::getSerial() const {
  if (not mMessage) {
    throw Exception("Cannot get serial from null message");
  }

  return dbus_message_get_serial(mMessage);
}

void Message::setSerial(dbus_uint32_t serial) {
  if (not mMessage) {
    throw Exception("Cannot set serial on null message");
  }

  dbus_message_set_serial(mMessage, serial);
}

Message::Message() {
}

} // namespace dashbus
