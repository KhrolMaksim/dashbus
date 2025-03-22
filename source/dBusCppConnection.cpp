#include <dBusCppConnection.h>

#include <iostream>
#include <stdexcept>

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  DBusCppServerHandler *serverHandler = static_cast<DBusCppServerHandler *>(user_data);

  const char *iface = dbus_message_get_interface(msg);
  if (not serverHandler->hasInterface(iface)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  const char *member = dbus_message_get_member(msg);
  if (not serverHandler->hasMethod(iface, member)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  MethodHandler handler = serverHandler->getMethodHandler(iface, member);
  return handler(conn, msg);
}

DBusCppConnection::DBusCppConnection(DBusBusType busType) {
  mConnection = dbus_bus_get(busType, mError);

  if (mError.isSet()) {
    throw std::runtime_error(mError.getMessage());
  }
}

DBusCppConnection::~DBusCppConnection() {
  dbus_connection_unref(mConnection);
}

void DBusCppConnection::requestName(const char *name, DBusNameFlag flags) {
  int ret = dbus_bus_request_name(mConnection, name, static_cast<unsigned int>(flags), mError);

  if (mError.isSet()) {
    throw std::runtime_error(mError.getMessage());
  }

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    throw std::runtime_error("Failed to request name");
  }
}

void DBusCppConnection::registerObjectPath(const char *path, DBusCppServerHandler &serverHandler) {
  DBusObjectPathVTable vtable = {NULL, &handle_message, NULL, NULL, NULL, NULL};
  if (!dbus_connection_register_object_path(mConnection, path, &vtable, &serverHandler)) {
    throw std::runtime_error("Failed to register object path");
  }
}

void DBusCppConnection::readWriteDispatch(int timeout) {
  dbus_connection_read_write_dispatch(mConnection, timeout);
}

std::thread DBusCppConnection::workProcess(int timeout) {
  return std::thread([this, timeout]() {
    while (true) {
      readWriteDispatch(timeout);
      std::cout << "Waiting for messages..." << std::endl;
    }
  });
}

DBusConnection *DBusCppConnection::get() const {
  return mConnection;
}
