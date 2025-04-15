#include <Exception.h>

DashBus::Exception::Exception(const std::string &what) : std::runtime_error(what) {
}

DashBus::NameRequestException::NameRequestException(const std::string &what) : Exception(what) {
}
