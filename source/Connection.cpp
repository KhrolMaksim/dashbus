#include <Connection.h>

#include <iostream>
#include <stdexcept>

namespace dashbus {
/**
 * @brief Обработчик входящих сообщений D-Bus
 *
 * @param conn Указатель на D-Bus соединение
 * @param msg Указатель на входящее сообщение
 * @param user_data Указатель на пользовательские данные (ServerHandler)
 * @return Результат обработки сообщения
 * @todo Расширить и преобразовать работу.
 * @todo Привязать к контексту выполнения.
 */
DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  if (not conn or not msg or not user_data) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  ServerHandler *serverHandler = static_cast<ServerHandler *>(user_data);
  const char *iface = dbus_message_get_interface(msg);
  const char *member = dbus_message_get_member(msg);

  if (not iface or not member or not serverHandler->hasInterface(iface) or
      not serverHandler->hasMethod(iface, member)) {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  MethodHandler handler = serverHandler->getMethodHandler(iface, member);

  // Создаем объекты Connection и Message, которые будут освобождены в своих деструкторах
  Connection connection = Connection::createByPointer(conn);
  Message message = Message::createByPointer(msg);

  return handler(connection, message);
}

Connection::Connection(DBusBusType busType) {
  mConnection = dbus_bus_get(busType, mError.get());
  if (not mConnection) {
    throw Exception("Failed to connect to D-Bus: " + mError.message());
  }
}

Connection::~Connection() {
  if (mConnection) {
    dbus_connection_unref(mConnection);
  }
}

Connection Connection::createByPointer(DBusConnection *connection) {
  if (not connection) {
    throw Exception("Cannot create Connection from null pointer");
  }

  Connection conn;
  conn.mConnection = dbus_connection_ref(connection);

  if (not conn.mConnection) {
    throw Exception("Failed to reference connection");
  }

  return conn;
}

void Connection::requestName(const char *name, DBusNameFlag flags) {
  if (not name) {
    throw Exception("Cannot request null name");
  }

  int ret =
      dbus_bus_request_name(mConnection, name, static_cast<unsigned int>(flags), mError.get());

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    if (mError.isSet()) {
      throw NameRequestException(mError.message());
    } else {
      throw NameRequestException("Failed to request name: " + std::to_string(ret));
    }
  }
}

void Connection::registerObjectPath(const char *path, ServerHandler &serverHandler) {
  if (not path) {
    throw Exception("Cannot register null object path");
  }

  DBusObjectPathVTable vtable = {NULL, &handle_message, NULL, NULL, NULL, NULL};

  if (not dbus_connection_register_object_path(mConnection, path, &vtable, &serverHandler)) {
    if (mError.isSet()) {
      throw Exception("Failed to register object path: " + mError.message());
    } else {
      throw Exception("Failed to register object path");
    }
  }
}

void Connection::readWriteDispatch(int timeout) {
  if (not mConnection) {
    throw Exception("Connection is not initialized");
  }

  dbus_connection_read_write_dispatch(mConnection, timeout);
  if (mError.isSet()) {
    throw Exception("Failed to dispatch: " + mError.message());
  }
}

std::thread Connection::workProcess(int timeout) {
  return std::thread([this, timeout]() {
    try {
      while (isConnected()) {
        readWriteDispatch(timeout);
      }
    } catch (const std::exception &e) {
      std::cerr << "Error in work process: " << e.what() << std::endl;
    }
  });
}

void Connection::sendMessage(Message &message) {
  if (not mConnection) {
    throw Exception("Connection is not initialized");
  }

  if (not dbus_connection_send(mConnection, message.get(), NULL)) {
    if (mError.isSet()) {
      throw Exception("Failed to send message: " + mError.message());
    } else {
      throw Exception("Failed to send message");
    }
  }

  dbus_connection_flush(mConnection);
}

Message Connection::sendMessage(Message &message, int timeout) {
  if (not mConnection) {
    throw Exception("Connection is not initialized");
  }

  DBusMessage *ret =
      dbus_connection_send_with_reply_and_block(mConnection, message.get(), timeout, mError.get());

  if (not ret) {
    if (mError.isSet()) {
      throw Exception("Failed to send message: " + mError.message());
    } else {
      throw Exception("Failed to send message");
    }
  }

  Message msg = Message::createByPointer(ret);
  msg.mType = Message::Type::METHOD_REPLY;
  dbus_message_unref(ret);

  return msg;
}

DBusConnection *Connection::get() const {
  return mConnection;
}

bool Connection::isConnected() const {
  return mConnection != NULL && dbus_connection_get_is_connected(mConnection);
}

Connection::Connection() {
}

} // namespace dashbus