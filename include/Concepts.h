#pragma once

#include <string>
#include <variant>
#include <vector>

#include <dbus/dbus.h>

/*
Сквозное покрытие типов в концептах

DBUS_TYPE_BYTE +
DBUS_TYPE_BOOLEAN +
DBUS_TYPE_INT16 +
DBUS_TYPE_UINT16 +
DBUS_TYPE_INT32 +
DBUS_TYPE_UINT32 +
DBUS_TYPE_INT64 +
DBUS_TYPE_UINT64 +
DBUS_TYPE_DOUBLE +
DBUS_TYPE_STRING +-
DBUS_TYPE_OBJECT_PATH -
DBUS_TYPE_SIGNATURE -
DBUS_TYPE_UNIX_FD -

DBUS_TYPE_ARRAY +
DBUS_TYPE_VARIANT +-

DBUS_TYPE_STRUCT +
DBUS_TYPE_DICT_ENTRY -
*/

namespace dashbus {
template <typename T>
concept Byte = std::is_same_v<T, std::byte>;

template <typename T>
concept UnsafeByte = Byte<T> or std::is_same_v<T, char> or std::is_same_v<T, signed char> or
                     std::is_same_v<T, unsigned char>;

template <typename T>
concept DBusBoolean = std::is_same_v<T, dbus_bool_t>;

template <typename T>
concept CppBoolean = std::is_same_v<T, bool>;

template <typename T>
concept Boolean = CppBoolean<T> or DBusBoolean<T>;

template <typename T>
concept SInt16 = std::is_same_v<T, int16_t> or std::is_same_v<T, short>;

template <typename T>
concept UInt16 = std::is_same_v<T, u_int16_t>;

template <typename T>
concept Int16 = SInt16<T> or UInt16<T>;

template <typename T>
concept SInt32 = std::is_same_v<T, int32_t> or std::is_same_v<T, int>;

template <typename T>
concept UInt32 = std::is_same_v<T, u_int32_t>;

template <typename T>
concept Int32 = SInt32<T> or UInt32<T>;

template <typename T>
concept SInt64 = std::is_same_v<T, int64_t> or std::is_same_v<T, long>;

template <typename T>
concept UInt64 = std::is_same_v<T, u_int64_t>;

template <typename T>
concept Int64 = SInt64<T> or UInt64<T>;

template <typename T>
concept Double = std::is_same_v<T, double>;

template <typename T>
concept UnsafeDouble = Double<T> or std::is_same_v<T, float>;

template <typename T>
concept String = std::is_same_v<T, std::string>;

template <typename T>
concept Array = requires(T vec) {
  typename T::value_type;
  typename T::allocator_type;
  vec.begin();
  vec.end();
  vec.size();
  vec[0];
  requires std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>;
};

template <typename T>
concept Variant = requires {
  typename std::variant_size<T>::type;
  requires std::variant_size_v<T> > 0;
};

/**
 @brief
 @todo Исправить концепт в соответствии с требованиями к классу имеющему возможность быть
 представленным dbus структурой
 **/
template <typename T>
concept ClassOrStruct = requires(T obj) {
  obj.getFieldsRef();
  requires std::is_class_v<T>;
};

template <typename T>
concept BaseDBusType =
    Byte<T> or Boolean<T> or Int16<T> or Int32<T> or Int64<T> or Double<T> or String<T>;

template <typename T>
concept CompoundDBusType = Array<T> or Variant<T>;

template <typename T>
concept StructDBusType = ClassOrStruct<T>;

template <typename T>
concept DBusArgument = BaseDBusType<T> or CompoundDBusType<T> or StructDBusType<T>;
} // namespace dashbus