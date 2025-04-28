#include <Message.h>

bool dashbus::MessageIter::initWriteIter(dashbus::Message &message) {
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

bool dashbus::MessageIter::initReadIter(dashbus::Message &message) {
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

dashbus::MessageIter::operator bool() {
  return isInit;
}

dashbus::MessageIter::operator DBusMessageIter *() {
  return &iter;
}

dashbus::Message::Message(const char *service, const char *path, const char *interface,
                          const char *method) {
  mMessage = dbus_message_new_method_call(service, path, interface, method);
  mType = Type::METHOD;
}

dashbus::Message::Message(const char *path, const char *interface, const char *name) {
  mMessage = dbus_message_new_signal(path, interface, name);
  mType = Type::SIGNAL;
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

void dashbus::Message::startWriteArguments() {
  mWriteIter.initWriteIter(*this);
}

void dashbus::Message::startReadArguments() {
  mReadIter.initReadIter(*this);
}

int dashbus::Message::getArgumentType() {
  if (not mReadIter) {
    startReadArguments();
  }

  return dbus_message_iter_get_arg_type(mReadIter);
}

dashbus::Message dashbus::Message::getReturnMessage() {
  Message message;
  message.mMessage = dbus_message_new_method_return(mMessage);
  message.mType = Type::METHOD_REPLY;

  return message;
}

dashbus::Message::Message() {
}
