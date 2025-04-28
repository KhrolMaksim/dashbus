#pragma once

#include <dbus/dbus.h>

#include <string>

#include <Exception.h>

namespace dashbus {

/**
 * @brief Класс для работы с ошибками DBus.
 *
 * Инкапсулирует DBusError, предоставляя безопасный интерфейс C++.
 */
class Error {
public:
  /**
   * @brief Конструктор по умолчанию. Инициализирует внутреннюю ошибку.
   */
  Error();

  /**
   * @brief Конструктор копирования.
   */
  Error(const Error &);

  /**
   * @brief Оператор присваивания.
   */
  Error &operator=(const Error &);

  /**
   * @brief Конструктор перемещения.
   * @param other Объект Error, из которого перемещается ошибка.
   */
  Error(Error &&other) noexcept;

  /**
   * @brief Оператор перемещения.
   * @param other Объект Error, из которого перемещается ошибка.
   * @return Ссылка на текущий объект.
   */
  Error &operator=(Error &&other) noexcept;

  /**
   * @brief Деструктор. Освобождает ресурсы DBusError.
   */
  ~Error();

  /**
   * @brief Проверяет, установлена ли ошибка.
   * @return true, если ошибка установлена.
   */
  bool isSet() const;

  /**
   * @brief Проверяет, совпадает ли имя ошибки с заданным.
   * @param name Имя для проверки.
   * @return true, если имя совпадает и ошибка установлена.
   */
  bool hasName(const std::string &name) const;

  /**
   * @brief Устанавливает ошибку с копированием строк.
   * @param name Имя ошибки.
   * @param message Сообщение ошибки.
   */
  void set(const std::string &name, const std::string &message);

  /**
   * @brief Устанавливает ошибку без копирования строк (для литералов).
   * @param name Имя ошибки.
   * @param message Сообщение ошибки.
   */
  void setConst(const std::string &name, const std::string &message);

  /**
   * @brief Возвращает сообщение ошибки.
   * @return Сообщение или пустая строка, если ошибки нет.
   */
  std::string message() const;

  /**
   * @brief Возвращает имя ошибки.
   * @return Имя или пустая строка, если ошибки нет.
   */
  std::string name() const;

  /**
   * @brief Сбрасывает ошибку в начальное состояние.
   */
  void clear();

  /**
   * @brief Возвращает указатель на DBusError.
   * @return Указатель на ошибку или nullptr, если ошибка не установлена.
   */
  DBusError *get() noexcept;

  /**
   * @brief Возвращает константный указатель на DBusError.
   * @return Константный указатель на ошибку или nullptr, если ошибка не установлена.
   */
  const DBusError *get() const noexcept;

  /**
   * @brief Бросает исключение NameRequestException, если ошибка установлена.
   * @throws NameRequestException Содержит имя и сообщение ошибки.
   */
  void throwIfSet() const;

private:
  /**
   * @brief Внутренний объект ошибки DBus.
   */
  DBusError mError;
};

} // namespace dashbus