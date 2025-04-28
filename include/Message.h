#pragma once

#include <dbus/dbus.h>

#include <Concepts.h>
#include <Exception.h>

#include "Detail.h"

namespace dashbus {

/**
 * @brief Класс для работы с итератором сообщений D-Bus
 *
 * Предоставляет безопасный интерфейс для работы с DBusMessageIter.
 * Используется для чтения и записи аргументов сообщений.
 */
class Message;
struct MessageIter {
  /**
   * @brief Инициализирует итератор для записи аргументов
   * @param message Сообщение для записи
   * @return true если инициализация успешна
   */
  bool initWriteIter(Message &);

  /**
   * @brief Инициализирует итератор для чтения аргументов
   * @param message Сообщение для чтения
   * @return true если инициализация успешна
   */
  bool initReadIter(Message &);

  /**
   * @brief Проверяет, инициализирован ли итератор
   * @return true если итератор инициализирован
   */
  operator bool();

  /**
   * @brief Возвращает указатель на DBusMessageIter
   * @return Указатель на DBusMessageIter
   */
  operator DBusMessageIter *();

private:
  DBusMessageIter iter;
  bool isInit = false;
};

/**
 * @brief Класс для работы с сообщениями D-Bus
 *
 * Предоставляет безопасный C++ интерфейс для работы с DBusMessage.
 * Поддерживает создание, чтение и запись сообщений различных типов.
 */
class Message {
public:
  /**
   * @brief Создает сообщение вызова метода
   * @param service Имя сервиса
   * @param path Путь объекта
   * @param interface Имя интерфейса
   * @param method Имя метода
   * @throws Exception если не удалось создать сообщение
   */
  Message(const char *service, const char *path, const char *interface, const char *method);

  /**
   * @brief Создает сигнальное сообщение
   * @param path Путь объекта
   * @param interface Имя интерфейса
   * @param name Имя сигнала
   * @throws Exception если не удалось создать сообщение
   */
  Message(const char *path, const char *interface, const char *name);

  /**
   * @brief Деструктор. Освобождает ресурсы сообщения
   */
  ~Message();

  /**
   * @brief Типы сообщений D-Bus
   */
  enum class Type { UNDEFINED, METHOD, METHOD_REPLY, SIGNAL };

  /**
   * @brief Создает объект Message из существующего DBusMessage
   * @param message Указатель на существующее сообщение
   * @return Объект Message
   */
  static Message createByPointer(DBusMessage *message);

  /**
   * @brief Возвращает указатель на DBusMessage
   * @return Указатель на DBusMessage
   */
  DBusMessage *get() const;

  /**
   * @brief Преобразует в константный указатель на DBusMessage
   * @return Константный указатель на DBusMessage
   */
  operator const DBusMessage *() const;

  /**
   * @brief Преобразует в указатель на DBusMessage
   * @return Указатель на DBusMessage
   */
  operator DBusMessage *();

  /**
   * @brief Начинает запись аргументов в сообщение
   * @throws Exception если сообщение не инициализировано
   */
  void startWriteArguments();

  /**
   * @brief Начинает чтение аргументов из сообщения
   * @throws Exception если сообщение не инициализировано
   */
  void startReadArguments();

  /**
   * @brief Добавляет аргумент в сообщение
   * @param value Значение аргумента
   * @throws Exception если сообщение не инициализировано
   */
  void addArgument(DBusArgument auto value);

  /**
   * @brief Получает аргумент из сообщения
   * @param value Переменная для сохранения значения
   * @throws Exception если сообщение не инициализировано
   */
  void getArgument(DBusArgument auto &value);

  /**
   * @brief Возвращает тип текущего аргумента
   * @return Тип аргумента
   * @throws Exception если сообщение не инициализировано
   */
  int getArgumentType();

  /**
   * @brief Создает сообщение-ответ на текущее сообщение
   * @return Сообщение-ответ
   * @throws Exception если сообщение не инициализировано
   */
  Message getReturnMessage();

  /**
   * @brief Возвращает серийный номер сообщения
   * @return Серийный номер сообщения
   * @throws Exception если сообщение не инициализировано
   */
  dbus_uint32_t getSerial() const;

  /**
   * @brief Устанавливает серийный номер сообщения
   * @param serial Серийный номер для установки
   * @throws Exception если сообщение не инициализировано
   */
  void setSerial(dbus_uint32_t serial);

  friend class Connection;

private:
  /**
   * @brief Конструктор по умолчанию
   */
  Message();

  Type mType = Type::UNDEFINED;

  /**
   * @brief Добавляет базовый аргумент в сообщение
   * @param msgIter Итератор сообщения
   * @param value Значение аргумента
   * @throws Exception если msgIter равен NULL
   */
  static void addMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto value);

  /**
   * @brief Добавляет составной аргумент в сообщение
   * @param msgIter Итератор сообщения
   * @param value Значение аргумента
   * @throws Exception если msgIter равен NULL
   */
  static void addMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto value);

  /**
   * @brief Добавляет структурный аргумент в сообщение
   * @param msgIter Итератор сообщения
   * @param value Значение аргумента
   * @throws Exception если msgIter равен NULL
   */
  static void addMessageArgument(DBusMessageIter *msgIter, StructDBusType auto value);

  /**
   * @brief Получает базовый аргумент из сообщения
   * @param msgIter Итератор сообщения
   * @param value Переменная для сохранения значения
   * @throws Exception если msgIter равен NULL
   */
  static void getMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto &value);

  /**
   * @brief Получает составной аргумент из сообщения
   * @param msgIter Итератор сообщения
   * @param value Переменная для сохранения значения
   * @throws Exception если msgIter равен NULL
   */
  static void getMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto &value);

  /**
   * @brief Получает структурный аргумент из сообщения
   * @param msgIter Итератор сообщения
   * @param value Переменная для сохранения значения
   * @throws Exception если msgIter равен NULL
   */
  static void getMessageArgument(DBusMessageIter *msgIter, StructDBusType auto &value);

private:
  DBusMessage *mMessage = nullptr;
  MessageIter mReadIter;
  MessageIter mWriteIter;
};
} // namespace dashbus

void dashbus::Message::addArgument(DBusArgument auto value) {
  if (not mWriteIter) {
    startWriteArguments();
  }

  dashbus::Message::addMessageArgument(mWriteIter, value);
}

void dashbus::Message::getArgument(DBusArgument auto &value) {
  if (not mReadIter) {
    startReadArguments();
  }

  dashbus::Message::getMessageArgument(mReadIter, value);
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto value) {
  if (not msgIter) {
    throw Exception("Cannot add argument to null iterator");
  }

  using CurrentType = std::decay_t<decltype(value)>;

  if constexpr (Byte<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BYTE, &value);
  } else if constexpr (DBusBoolean<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BOOLEAN, &value);
  } else if constexpr (CppBoolean<CurrentType>) {
    dbus_bool_t temp = value ? TRUE : FALSE;
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_BOOLEAN, &temp);
  } else if constexpr (SInt16<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT16, &value);
  } else if constexpr (UInt16<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT16, &value);
  } else if constexpr (SInt32<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT32, &value);
  } else if constexpr (UInt32<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT32, &value);
  } else if constexpr (SInt64<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_INT64, &value);
  } else if constexpr (UInt64<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_UINT64, &value);
  } else if constexpr (Double<CurrentType>) {
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_DOUBLE, &value);
  } else if constexpr (String<CurrentType>) {
    const char *c_str = value.c_str();
    dbus_message_iter_append_basic(msgIter, DBUS_TYPE_STRING, &c_str);
  } else {
    static_assert(false, "Выполнен вызов addMessageArgument для необработанного BaseDBusType");
  }
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto value) {
  using CurrentType = std::decay_t<decltype(value)>;
  if constexpr (Array<CurrentType>) {
    using ElementType = typename CurrentType::value_type;
    ElementType fictiveElement;

    std::string elem_sig = detail::getDBusSignature(fictiveElement);
    const char *elem_sig_c_str = elem_sig.c_str();

    DBusMessageIter arrIter;
    dbus_message_iter_open_container(msgIter, DBUS_TYPE_ARRAY, elem_sig_c_str, &arrIter);

    for (auto const &elem : value) {
      Message::addMessageArgument(&arrIter, elem);
    }

    dbus_message_iter_close_container(msgIter, &arrIter);
  } else {
    static_assert(false, "Выполнен вызов addMessageArgument для необработанного CompoundDBusType");
  }
}

void dashbus::Message::addMessageArgument(DBusMessageIter *msgIter, StructDBusType auto value) {
  DBusMessageIter structIter;
  dbus_message_iter_open_container(msgIter, DBUS_TYPE_STRUCT, NULL, &structIter);

  auto addMessageArgumentTuple = [&structIter](auto &&...args) {
    (Message::addMessageArgument(&structIter, args), ...);
  };

  auto fieldRefs = value.getFieldsRef();
  std::apply(addMessageArgumentTuple, fieldRefs);

  dbus_message_iter_close_container(msgIter, &structIter);
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, BaseDBusType auto &value) {
  using CurrentType = std::decay_t<decltype(value)>;

  if constexpr (Byte<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (DBusBoolean<CurrentType> or CppBoolean<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt16<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt16<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt32<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt32<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (SInt64<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (UInt64<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (Double<CurrentType>) {
    dbus_message_iter_get_basic(msgIter, &value);
  } else if constexpr (String<CurrentType>) {
    const char *tempValue;
    dbus_message_iter_get_basic(msgIter, &tempValue);
    value = std::string(tempValue);
  } else {
    static_assert(false, "Выполнен вызов getMessageArgument для необработанного BaseDBusType");
  }

  dbus_message_iter_next(msgIter);
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, CompoundDBusType auto &value) {
  using Type = std::decay_t<decltype(value)>;
  if constexpr (Array<Type>) {
    using ElementType = typename Type::value_type;

    DBusMessageIter subIter;
    dbus_message_iter_recurse(msgIter, &subIter);

    value.clear();

    while (dbus_message_iter_get_arg_type(&subIter) != DBUS_TYPE_INVALID) {
      ElementType elem;
      Message::getMessageArgument(&subIter, elem);
      value.push_back(std::move(elem));
      // Сдвигом после элементов возложено на сами элементы
      // dbus_message_iter_next(&subIter);
    }

    dbus_message_iter_next(msgIter);
  } else {
    static_assert(false, "Unsupported CompoundDBusType");
  }
}

void dashbus::Message::getMessageArgument(DBusMessageIter *msgIter, StructDBusType auto &value) {
  DBusMessageIter structIter;
  dbus_message_iter_recurse(msgIter, &structIter);

  auto addMessageArgumentTuple = [&structIter](auto &&...args) {
    (Message::getMessageArgument(&structIter, args), ...);
  };

  auto fieldRefs = value.getFieldsRef();
  std::apply(addMessageArgumentTuple, fieldRefs);

  dbus_message_iter_next(msgIter);
}