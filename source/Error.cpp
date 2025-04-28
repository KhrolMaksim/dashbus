#include <Error.h>

namespace dashbus {

Error::Error() {
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

Error::Error(Error &&other) noexcept {
  dbus_error_init(&mError);

  if (other.isSet()) {
    dbus_move_error(&other.mError, &mError);
  }
}

Error &Error::operator=(Error &&other) noexcept {
  if (this != &other) {
    clear();

    if (other.isSet()) {
      dbus_move_error(&other.mError, &mError);
    }
  }

  return *this;
}

Error::~Error() {
  dbus_error_free(&mError);
}

bool Error::isSet() const {
  return dbus_error_is_set(&mError);
}

bool Error::hasName(const std::string &name) const {
  return isSet() && not name.empty() && dbus_error_has_name(&mError, name.c_str());
}

void Error::set(const std::string &name, const std::string &message) {
  clear();

  if (not name.empty() && not message.empty()) {
    dbus_set_error(&mError, name.c_str(), "%s", message.c_str());
  }
}

void Error::setConst(const std::string &name, const std::string &message) {
  clear();

  if (not name.empty() && not message.empty()) {
    dbus_set_error_const(&mError, name.c_str(), message.c_str());
  }
}

std::string Error::message() const {
  return isSet() ? mError.message : "";
}

std::string Error::name() const {
  return isSet() ? mError.name : "";
}

void Error::clear() {
  dbus_error_free(&mError);
  dbus_error_init(&mError);
}

DBusError *Error::get() noexcept {
  return &mError;
}

const DBusError *Error::get() const noexcept {
  return &mError;
}

void Error::throwIfSet() const {
  if (isSet()) {
    throw NameRequestException("DBus error: " + name() + " - " + message());
  }
}

} // namespace dashbus