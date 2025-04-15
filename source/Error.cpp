#include <Error.h>

/// @brief Класс, описывающий ошибки в работе с dbus
DashBus::Error::Error() {
  dbus_error_init(&mError);
}

DashBus::Error::Error(const Error &other) {
  dbus_error_init(&mError);

  if (other.isSet()) {
    set(other.name(), other.message());
  }
}

DashBus::Error &DashBus::Error::operator=(const Error &other) {
  if (this != &other) {
    clear();

    if (other.isSet()) {
      set(other.name(), other.message());
    }
  }
  return *this;
}

DashBus::Error::Error(Error &&other) noexcept {
  dbus_error_init(&mError);

  if (other.isSet()) {
    dbus_move_error(&other.mError, &mError);
  }
}

DashBus::Error &DashBus::Error::operator=(Error &&other) noexcept {
  if (this != &other) {
    clear();
    if (other.isSet()) {
      dbus_move_error(&other.mError, &mError);
    }
  }
  return *this;
}

DashBus::Error::~Error() {
  dbus_error_free(&mError);
}

bool DashBus::Error::isSet() const {
  return dbus_error_is_set(&mError);
}

bool DashBus::Error::hasName(const std::string &name) const {
  return dbus_error_has_name(&mError, name.c_str());
}

void DashBus::Error::set(const std::string &name, const std::string &message) {
  dbus_set_error(&mError, name.c_str(), "%s", message.c_str());
}

void DashBus::Error::setConst(const std::string &name, const std::string &message) {
  dbus_set_error_const(&mError, name.c_str(), message.c_str());
}

std::string DashBus::Error::message() const {
  return isSet() ? mError.message : "";
}

std::string DashBus::Error::name() const {
  return isSet() ? mError.name : "";
}

void DashBus::Error::clear() {
  dbus_error_free(&mError);
  dbus_error_init(&mError);
}

DBusError *DashBus::Error::get() {
  return &mError;
}

const DBusError *DashBus::Error::get() const {
  return &mError;
}

void DashBus::Error::throwIfSet() const {
  if (isSet()) {
    throw NameRequestException("DBus error: " + name() + " - " + message());
  }
}

DashBus::Error::operator const DBusError *() const {
  return &mError;
}

DashBus::Error::operator DBusError *() {
  return &mError;
}