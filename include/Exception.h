#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace dashbus {

/**
 * @brief Базовый класс исключений для dashbus.
 */
class Exception : public std::runtime_error {
public:
  /**
   * @brief Конструктор исключения с сообщением.
   * @param what Сообщение об ошибке.
   */
  explicit Exception(std::string_view what) noexcept;
  explicit Exception(const std::string &what) noexcept;
  explicit Exception(const char *what) noexcept;
};

/**
 * @brief Исключение, связанное с некорректным запросом имени в DBus.
 */
class NameRequestException : public Exception {
public:
  /**
   * @brief Конструктор исключения с сообщением.
   * @param what Сообщение об ошибке.
   */
  explicit NameRequestException(std::string_view what) noexcept;
  explicit NameRequestException(const std::string &what) noexcept;
  explicit NameRequestException(const char *what) noexcept;
};

} // namespace dashbus