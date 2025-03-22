#include <dBusCppConnection.h>

#include <stdexcept>

DBusCppConnection::DBusCppConnection(DBusBusType busType) {
  mConnection = dbus_bus_get(busType, mError);

  if (mError.isSet()) {
    throw std::runtime_error(mError.getMessage());
  }
}

DBusCppConnection::~DBusCppConnection() {
  dbus_connection_unref(mConnection);
}

DBusConnection *DBusCppConnection::get() const {
  return mConnection;
}
