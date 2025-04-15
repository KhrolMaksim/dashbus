#pragma once

#include <stdexcept>
#include <string>

class DBusCppException : public std::runtime_error {
public:
  DBusCppException(const std::string &what);
};

class DBusCppNameRequestException : public DBusCppException {
public:
  DBusCppNameRequestException(const std::string &what);
};