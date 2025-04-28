#pragma once

#include <dbus/dbus.h>

#include <cstring>
#include <map>
#include <mutex>

namespace dashbus {

/**
 * @brief Функтор для сравнения C-строк в контейнерах
 *
 * Используется для корректного сравнения C-строк в std::map
 */
struct CmpStr {
  /**
   * @brief Оператор сравнения для C-строк
   *
   * @param a Первая строка
   * @param b Вторая строка
   * @return true если a < b
   */
  bool operator()(char const *a, char const *b) const {
    return std::strcmp(a, b) < 0;
  }
};

class Connection;
class Message;

/**
 * @brief Тип функции-обработчика метода D-Bus
 *
 * @param connection Соединение D-Bus
 * @param message Сообщение D-Bus
 * @return Результат обработки сообщения
 */
using MethodHandler = DBusHandlerResult (*)(Connection &, Message &);

/**
 * @brief Тип контейнера для хранения обработчиков методов
 *
 * Ключ - имя метода, значение - обработчик
 */
using MethodHandlers = std::map<const char *, MethodHandler, CmpStr>;

/**
 * @brief Тип контейнера для хранения обработчиков интерфейсов
 *
 * Ключ - имя интерфейса, значение - контейнер обработчиков методов
 */
using InterfaceHandlers = std::map<const char *, MethodHandlers, CmpStr>;

/**
 * @brief Класс для управления обработчиками D-Bus
 *
 * Предоставляет интерфейс для регистрации и управления обработчиками
 * интерфейсов и методов D-Bus. Обеспечивает потокобезопасность
 * при работе с обработчиками.
 */
class ServerHandler {
public:
  /**
   * @brief Конструктор по умолчанию
   */
  ServerHandler();

  /**
   * @brief Деструктор
   */
  ~ServerHandler();

  /**
   * @brief Добавляет интерфейс с обработчиками методов
   *
   * @param iface Имя интерфейса
   * @param handlers Контейнер обработчиков методов
   */
  void addInterface(const char *iface, const MethodHandlers &handlers);

  /**
   * @brief Удаляет интерфейс и все его обработчики
   *
   * @param iface Имя интерфейса
   */
  void removeInterface(const char *iface);

  /**
   * @brief Добавляет обработчик метода
   *
   * @param iface Имя интерфейса
   * @param method Имя метода
   * @param handler Обработчик метода
   */
  void addMethod(const char *iface, const char *method, MethodHandler handler);

  /**
   * @brief Удаляет обработчик метода
   *
   * @param iface Имя интерфейса
   * @param method Имя метода
   */
  void removeMethod(const char *iface, const char *method);

  /**
   * @brief Проверяет наличие интерфейса
   *
   * @param iface Имя интерфейса
   * @return true если интерфейс зарегистрирован
   */
  bool hasInterface(const char *iface) const;

  /**
   * @brief Проверяет наличие метода в интерфейсе
   *
   * @param iface Имя интерфейса
   * @param method Имя метода
   * @return true если метод зарегистрирован в интерфейсе
   */
  bool hasMethod(const char *iface, const char *method) const;

  /**
   * @brief Возвращает все зарегистрированные интерфейсы и их обработчики
   *
   * @return Контейнер интерфейсов и их обработчиков
   */
  InterfaceHandlers getInterfaceHandlers() const;

  /**
   * @brief Возвращает обработчики методов для указанного интерфейса
   *
   * @param iface Имя интерфейса
   * @return Контейнер обработчиков методов
   */
  MethodHandlers getMethodHandlers(const char *iface) const;

  /**
   * @brief Возвращает обработчик указанного метода
   *
   * @param iface Имя интерфейса
   * @param method Имя метода
   * @return Обработчик метода
   */
  MethodHandler getMethodHandler(const char *iface, const char *method) const;

private:
  InterfaceHandlers mInterfaceHandlers;
  mutable std::mutex mHandlersMutex;
};
} // namespace dashbus