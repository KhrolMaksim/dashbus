#include "dBusMessage.h"

#include <iostream>

DBusMessage *DBusCppMessageBase::get() const {
  return mMessage;
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
