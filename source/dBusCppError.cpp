#include <dBusCppError.h>

#include <stdexcept>

/// @brief Класс, описывающий ошибки в работе с dbus
DBusCppError::DBusCppError() {
  dbus_error_init(&mError);
}

DBusCppError::DBusCppError(const DBusCppError &other) {
  dbus_error_init(&mError);

  if (other.isSet()) {
    set(other.name(), other.message());
  }
}

DBusCppError &DBusCppError::operator=(const DBusCppError &other) {
  if (this != &other) {
    clear();

    if (other.isSet()) {
      set(other.name(), other.message());
    }
  }
  return *this;
}

DBusCppError::DBusCppError(DBusCppError &&other) noexcept {
  dbus_error_init(&mError);

  if (other.isSet()) {
    dbus_move_error(&other.mError, &mError);
  }
}

DBusCppError &DBusCppError::operator=(DBusCppError &&other) noexcept {
  if (this != &other) {
    clear();
    if (other.isSet()) {
      dbus_move_error(&other.mError, &mError);
    }
  }
  return *this;
}

DBusCppError::~DBusCppError() {
  dbus_error_free(&mError);
}

bool DBusCppError::isSet() const {
  return dbus_error_is_set(&mError);
}

bool DBusCppError::hasName(const std::string &name) const {
  return dbus_error_has_name(&mError, name.c_str());
}

void DBusCppError::set(const std::string &name, const std::string &message) {
  dbus_set_error(&mError, name.c_str(), "%s", message.c_str());
}

void DBusCppError::setConst(const std::string &name, const std::string &message) {
  dbus_set_error_const(&mError, name.c_str(), message.c_str());
}

std::string DBusCppError::message() const {
  return isSet() ? mError.message : "";
}

std::string DBusCppError::name() const {
  return isSet() ? mError.name : "";
}

void DBusCppError::clear() {
  dbus_error_free(&mError);
  dbus_error_init(&mError);
}

DBusError *DBusCppError::get() {
  return &mError;
}

const DBusError *DBusCppError::get() const {
  return &mError;
}

void DBusCppError::throwIfSet() const {
  if (isSet()) {
    throw std::runtime_error("DBus error: " + name() + " - " + message());
  }
}

DBusCppError::operator const DBusError *() const {
  return &mError;
}

DBusCppError::operator DBusError *() {
  return &mError;
}