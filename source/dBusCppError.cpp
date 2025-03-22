#include <dBusCppError.h>

DBusCppError::DBusCppError() {
  dbus_error_init(&mError);
}

DBusCppError::~DBusCppError() {
  dbus_error_free(&mError);
}

dbus_bool_t DBusCppError::isSet() const {
  return dbus_error_is_set(&mError);
}

const char *DBusCppError::getMessage() const {
  return mError.message;
}

// const DBusError *DBusCppError::get() const {
//   return &mError;
// }

DBusCppError::operator const DBusError *() const {
  return &mError;
}

DBusCppError::operator DBusError *() {
  return &mError;
}
