#include <Exception.h>

namespace dashbus {

Exception::Exception(std::string_view what) noexcept : std::runtime_error(std::string(what)) {
}

Exception::Exception(const std::string &what) noexcept : std::runtime_error(what) {
}

Exception::Exception(const char *what) noexcept : std::runtime_error(what) {
}

NameRequestException::NameRequestException(std::string_view what) noexcept : Exception(what) {
}

NameRequestException::NameRequestException(const std::string &what) noexcept : Exception(what) {
}

NameRequestException::NameRequestException(const char *what) noexcept : Exception(what) {
}

} // namespace dashbus