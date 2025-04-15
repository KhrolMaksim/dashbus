#include <Exception.h>

dashbus::Exception::Exception(const std::string &what) : std::runtime_error(what) {
}

dashbus::NameRequestException::NameRequestException(const std::string &what) : Exception(what) {
}
