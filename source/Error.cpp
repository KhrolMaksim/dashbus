#include <Error.h>

/// @brief Класс, описывающий ошибки в работе с dbus
dashbus::Error::Error() {
  dbus_error_init(&mError);
}

dashbus::Error::Error(const Error &other) {
  dbus_error_init(&mError);

  if (other.isSet()) {
    set(other.name(), other.message());
  }
}

dashbus::Error &dashbus::Error::operator=(const Error &other) {
  if (this != &other) {
    clear();

    if (other.isSet()) {
      set(other.name(), other.message());
    }
  }
  return *this;
}

dashbus::Error::Error(Error &&other) noexcept {
  dbus_error_init(&mError);

  if (other.isSet()) {
    dbus_move_error(&other.mError, &mError);
  }
}

dashbus::Error &dashbus::Error::operator=(Error &&other) noexcept {
  if (this != &other) {
    clear();
    if (other.isSet()) {
      dbus_move_error(&other.mError, &mError);
    }
  }
  return *this;
}

dashbus::Error::~Error() {
  dbus_error_free(&mError);
}

bool dashbus::Error::isSet() const {
  return dbus_error_is_set(&mError);
}

bool dashbus::Error::hasName(const std::string &name) const {
  return dbus_error_has_name(&mError, name.c_str());
}

void dashbus::Error::set(const std::string &name, const std::string &message) {
  dbus_set_error(&mError, name.c_str(), "%s", message.c_str());
}

void dashbus::Error::setConst(const std::string &name, const std::string &message) {
  dbus_set_error_const(&mError, name.c_str(), message.c_str());
}

std::string dashbus::Error::message() const {
  return isSet() ? mError.message : "";
}

std::string dashbus::Error::name() const {
  return isSet() ? mError.name : "";
}

void dashbus::Error::clear() {
  dbus_error_free(&mError);
  dbus_error_init(&mError);
}

DBusError *dashbus::Error::get() {
  return &mError;
}

const DBusError *dashbus::Error::get() const {
  return &mError;
}

void dashbus::Error::throwIfSet() const {
  if (isSet()) {
    throw NameRequestException("DBus error: " + name() + " - " + message());
  }
}

dashbus::Error::operator const DBusError *() const {
  return &mError;
}

dashbus::Error::operator DBusError *() {
  return &mError;
}