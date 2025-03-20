#include "dBusServer.h"

#include <iostream>
#include <thread>
DBusCppServer::DBusCppServer(std::string name, DBusBusType type)
    : DBusCppServer(name, DBUS_NAME_FLAG_REPLACE_EXISTING, type) {
}

DBusCppServer::DBusCppServer(std::string name, int flags, DBusBusType type) : mConnection(type) {
  dbus_error_init(&mError);

  int ret = dbus_bus_request_name(mConnection.get(), name.c_str(), flags, &mError);

  if (dbus_error_is_set(&mError)) {
    std::cerr << "Failed to request name: " << mError.message << std::endl;
  }

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    std::cerr << "Failed to request name: " << ret << std::endl;
  }

  auto filter = [](DBusConnection *connection, DBusMessage *message, void *data) {
    std::cout << "Received message: " << dbus_message_get_member(message) << std::endl;
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  };

  auto resultAddFilter = dbus_connection_add_filter(mConnection.get(), filter, nullptr, nullptr);

  if (not resultAddFilter) {
    std::cerr << "Failed to add filter: " << resultAddFilter << std::endl;
  }
}

DBusCppServer::~DBusCppServer() {
  dbus_error_free(&mError);
}

void DBusCppServer::run() {
  std::thread([this]() { this->run(); }).detach();
  while (true) {
    dbus_connection_read_write_dispatch(mConnection.get(), -1);
  }
}
