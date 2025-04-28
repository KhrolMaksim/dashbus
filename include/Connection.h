#pragma once

#include <dbus/dbus.h>

#include <thread>

#include <Error.h>
#include <Message.h>
#include <ServerHandler.h>

namespace dashbus {

/**
 * @brief Флаги для запроса имени в D-Bus
 */
enum class DBusNameFlag {
  ALLOW_REPLACEMENT = DBUS_NAME_FLAG_ALLOW_REPLACEMENT,
  REPLACE_EXISTING = DBUS_NAME_FLAG_REPLACE_EXISTING,
  DO_NOT_QUEUE = DBUS_NAME_FLAG_DO_NOT_QUEUE,
};

/**
 * @brief Класс для работы с D-Bus соединением
 *
 * Предоставляет безопасный C++ интерфейс для работы с D-Bus соединениями.
 * Управляет жизненным циклом соединения и предоставляет методы для
 * взаимодействия с D-Bus системой.
 */
class Connection {
public:
  /**
   * @brief Создает новое соединение с указанным типом шины
   * @param busType Тип D-Bus шины (SESSION/SYSTEM/STARTER)
   * @throws Exception если не удалось установить соединение
   */
  Connection(DBusBusType busType);

  /**
   * @brief Деструктор. Освобождает ресурсы D-Bus соединения
   */
  ~Connection();

  /**
   * @brief Создает объект Connection из существующего D-Bus соединения
   * @param connection Указатель на существующее D-Bus соединение
   * @return Объект Connection
   * @throws Exception если connection равен NULL
   */
  static Connection createByPointer(DBusConnection *connection);

  /**
   * @brief Запрашивает имя в D-Bus системе
   * @param name Имя для запроса
   * @param flags Флаги запроса имени
   * @throws Exception если name равен NULL
   * @throws NameRequestException если не удалось запросить имя
   */
  void requestName(const char *name, DBusNameFlag flags);

  /**
   * @brief Регистрирует обработчик для указанного пути объекта
   * @param path Путь объекта
   * @param serverHandler Обработчик для этого пути
   * @throws Exception если path равен NULL или не удалось зарегистрировать путь
   */
  void registerObjectPath(const char *path, ServerHandler &serverHandler);

  /**
   * @brief Обрабатывает входящие и исходящие сообщения
   * @param timeout Таймаут в миллисекундах
   * @throws Exception если соединение не инициализировано или произошла ошибка при обработке
   */
  void readWriteDispatch(int timeout);

  /**
   * @brief Запускает обработку сообщений в отдельном потоке
   * @param timeout Таймаут в миллисекундах
   * @return Поток обработки сообщений
   */
  std::thread workProcess(int timeout);

  /**
   * @brief Отправляет сообщение без ожидания ответа
   * @param message Сообщение для отправки
   * @throws Exception если соединение не инициализировано или не удалось отправить сообщение
   */
  void sendMessage(Message &message);

  /**
   * @brief Отправляет сообщение и ждет ответа
   * @param message Сообщение для отправки
   * @param timeout Таймаут ожидания в миллисекундах
   * @return Ответное сообщение
   * @throws Exception если соединение не инициализировано или не удалось отправить сообщение
   */
  Message sendMessage(Message &message, int timeout);

  /**
   * @brief Возвращает указатель на D-Bus соединение
   * @return Указатель на D-Bus соединение
   */
  DBusConnection *get() const;

  /**
   * @brief Проверяет, установлено ли соединение
   * @return true если соединение установлено
   */
  bool isConnected() const;

private:
  /**
   * @brief Конструктор по умолчанию
   */
  Connection();

private:
  DBusConnection *mConnection;
  Error mError;
};
} // namespace dashbus