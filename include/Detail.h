#pragma once

#include <type_traits>

#include <dbus/dbus.h>

#include <Concepts.h>

namespace dashbus {
namespace detail {

std::string getDBusSignature(Byte auto &value) {
  return DBUS_TYPE_BYTE_AS_STRING;
}

std::string getDBusSignature(Boolean auto &value) {
  return DBUS_TYPE_BOOLEAN_AS_STRING;
}

std::string getDBusSignature(SInt16 auto &value) {
  return DBUS_TYPE_INT16_AS_STRING;
}

std::string getDBusSignature(UInt16 auto &value) {
  return DBUS_TYPE_UINT16_AS_STRING;
}

std::string getDBusSignature(SInt32 auto &value) {
  return DBUS_TYPE_INT32_AS_STRING;
}

std::string getDBusSignature(UInt32 auto &value) {
  return DBUS_TYPE_UINT32_AS_STRING;
}

std::string getDBusSignature(SInt64 auto &value) {
  return DBUS_TYPE_INT64_AS_STRING;
}

std::string getDBusSignature(UInt64 auto &value) {
  return DBUS_TYPE_UINT64_AS_STRING;
}

std::string getDBusSignature(Double auto &value) {
  return DBUS_TYPE_DOUBLE_AS_STRING;
}

std::string getDBusSignature(String auto &value) {
  return DBUS_TYPE_STRING_AS_STRING;
}

std::string getDBusSignature(ClassOrStruct auto &value);

std::string getDBusSignature(Array auto &value) {
  using ElementType = typename std::decay_t<decltype(value)>::value_type;
  ElementType temp;
  std::string result = DBUS_TYPE_ARRAY_AS_STRING;
  result += getDBusSignature(temp);
  return result;
}

std::string getDBusSignature(ClassOrStruct auto &value) {
  std::string signature = "(";
  auto fields = value.getFieldsRef();

  auto processField = [&](auto &&field) { signature += getDBusSignature(field); };

  std::apply([&](auto &&...fields) { (processField(fields), ...); }, fields);

  signature += ")";
  return signature;
}
} // namespace detail
} // namespace dashbus