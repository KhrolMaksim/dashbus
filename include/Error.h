#pragma once

#include <dbus/dbus.h>

#include <string>

#include <Exception.h>

namespace DashBus {
class Error {
public:
  Error();

  /// @todo В конечном варианте не должно быть копирующих конструкторов, если только не будет найден
  /// хороший способ проводить это без больших проблем
  Error(const Error &);

  /// @todo В конечном варианте не должно быть копирующих конструкторов, если только не будет найден
  /// хороший способ проводить это без больших проблем
  Error &operator=(const Error &);

  Error(Error &&other) noexcept;
  Error &operator=(Error &&other) noexcept;

  ~Error();

  /// @brief проверка на наличие ошибки
  /// @return наличие ошибки
  bool isSet() const;

  /// @brief проверка на наличие ошибки по имени
  /// @param name проверяемое имя
  /// @return наличие ошибки
  bool hasName(const std::string &name) const;

  /// @brief установка ошибки
  /// @param name имя ошибки
  /// @param message сообщение ошибки
  void set(const std::string &name, const std::string &message);

  /// @brief установка ошибки
  /// @param name имя ошибки
  /// @param message сообщение ошибки
  void setConst(const std::string &name, const std::string &message);

  /// @brief получить сообщение ошибки
  /// @return сообщение ошибки
  std::string message() const;

  /// @brief получить имя ошибки
  /// @return имя ошибки
  std::string name() const;

  /// @brief очистка ошибки (сброс)
  void clear();

  /// @brief получение DBusError (сырой указатель ошибки)
  /// @return указатель ошибки
  DBusError *get();

  /// @brief получение DBusError (константный сырой указатель ошибки)
  /// @return константный указатель ошибки
  const DBusError *get() const;

  /// @brief бросает исключение типа NameRequestException содержащий
  /// сообщение с названием и сообщением DBusError
  void throwIfSet() const;

  /// @brief Преобразование в DBusError
  /// @todo В конечном варианте не должно быть неявных преобразований в базовые примитивы dbus
  operator const DBusError *() const;

  /// @brief Преобразование в DBusError*
  /// @todo В конечном варианте не должно быть неявных преобразований в базовые примитивы dbus
  operator DBusError *();

private:
  DBusError mError;
};
} // namespace DashBus