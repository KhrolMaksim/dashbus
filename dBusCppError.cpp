#include "dBusCppError.h"

DBusCppError::DBusCppError() {
  dbus_error_init(&mError);
}

DBusCppError::~DBusCppError() {
  dbus_error_free(&mError);
}

DBusError *DBusCppError::get() {
  return &mError;
}
