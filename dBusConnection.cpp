#include "dBusConnection.h"

#include <iostream>

DBusCppConnection::DBusCppConnection(DBusBusType connectionType) {
  dbus_error_init(&mError);

  mConnection = dbus_bus_get(connectionType, &mError);

  if (dbus_error_is_set(&mError)) {
    std::cerr << "Failed to connect to D-Bus: " << mError.message << std::endl;
    return;
  }

  if (mConnection == nullptr) {
    std::cerr << "Failed to connect to D-Bus" << std::endl;
    return;
  }
}

DBusCppConnection::~DBusCppConnection() {
  dbus_error_free(&mError);

  if (mConnection) {
    dbus_connection_unref(mConnection);
  }
}

DBusCppConnection::DBusCppConnection(const DBusCppConnection &other) {
  if (other.mConnection) {
    mConnection = dbus_connection_ref(other.mConnection);
  } else {
    mConnection = nullptr;

    std::cerr << "Failed copy to D-Bus connection" << std::endl;
  }
}

DBusCppConnection &DBusCppConnection::operator=(const DBusCppConnection &other) {
  if (this != &other and other.mConnection) {
    mConnection = dbus_connection_ref(other.mConnection);
  } else {
    mConnection = nullptr;

    std::cerr << "Failed copy to D-Bus connection" << std::endl;
  }

  return *this;
}

DBusConnection *DBusCppConnection::get() const {
  return mConnection;
}
