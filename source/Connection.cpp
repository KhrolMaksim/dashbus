#include <Connection.h>

#include <iostream>
#include <stdexcept>

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  dashbus::ServerHandler *serverHandler = static_cast<dashbus::ServerHandler *>(user_data);

  const char *iface = dbus_message_get_interface(msg);
  if (not serverHandler->hasInterface(iface)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  const char *member = dbus_message_get_member(msg);
  if (not serverHandler->hasMethod(iface, member)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  dashbus::MethodHandler handler = serverHandler->getMethodHandler(iface, member);
  return handler(conn, msg);
}

dashbus::Connection::Connection(DBusBusType busType) {
  mConnection = dbus_bus_get(busType, mError);

  if (mError.isSet()) {
    throw NameRequestException(mError.message());
  }
}

dashbus::Connection::~Connection() {
  dbus_connection_unref(mConnection);
}

dashbus::Connection dashbus::Connection::createByPointer(DBusConnection *connection) {
  Connection conn;

  conn.mConnection = dbus_connection_ref(connection);

  return conn;
}

void dashbus::Connection::requestName(const char *name, DBusNameFlag flags) {
  int ret = dbus_bus_request_name(mConnection, name, static_cast<unsigned int>(flags), mError);

  if (mError.isSet()) {
    throw NameRequestException(mError.message());
  }

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    throw NameRequestException("Failed to request name");
  }
}

void dashbus::Connection::registerObjectPath(const char *path, ServerHandler &serverHandler) {
  DBusObjectPathVTable vtable = {NULL, &handle_message, NULL, NULL, NULL, NULL};
  if (!dbus_connection_register_object_path(mConnection, path, &vtable, &serverHandler)) {
    throw NameRequestException("Failed to register object path");
  }
}

void dashbus::Connection::readWriteDispatch(int timeout) {
  dbus_connection_read_write_dispatch(mConnection, timeout);
}

std::thread dashbus::Connection::workProcess(int timeout) {
  return std::thread([this, timeout]() {
    while (true) {
      readWriteDispatch(timeout);
      std::cout << "Waiting for messages..." << std::endl;
    }
  });
}

void dashbus::Connection::sendMessage(Message &message) {
  dbus_bool_t ret = dbus_connection_send(mConnection, message.get(), NULL);

  if (not ret) {
    throw NameRequestException("Failed to send message");
  }

  dbus_connection_flush(mConnection);
}

DBusConnection *dashbus::Connection::get() const {
  return mConnection;
}

dashbus::Connection::Connection() {
}