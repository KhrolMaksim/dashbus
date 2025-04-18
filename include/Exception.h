#pragma once

#include <stdexcept>
#include <string>

namespace dashbus {
class Exception : public std::runtime_error {
public:
  Exception(const std::string &what);
};

class NameRequestException : public Exception {
public:
  NameRequestException(const std::string &what);
};
} // namespace dashbus