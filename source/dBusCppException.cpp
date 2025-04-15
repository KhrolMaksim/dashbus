#include <dBusCppException.h>

DBusCppException::DBusCppException(const std::string &what) : std::runtime_error(what) {
}

DBusCppNameRequestException::DBusCppNameRequestException(const std::string &what)
    : DBusCppException(what) {
}
