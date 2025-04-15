#pragma once

#include <dbus/dbus.h>

#include <cstring>
#include <map>
#include <mutex>

struct CmpStr {
  bool operator()(char const *a, char const *b) const {
    return std::strcmp(a, b) < 0;
  }
};

using MethodHandler = DBusHandlerResult (*)(DBusConnection *, DBusMessage *);
using MethodHandlers = std::map<const char *, MethodHandler, CmpStr>;
using InterfaceHandlers = std::map<const char *, MethodHandlers, CmpStr>;

class DBusCppServerHandler {
public:
  DBusCppServerHandler();
  ~DBusCppServerHandler();

  void addInterface(const char *iface, const MethodHandlers &handlers);
  void removeInterface(const char *iface);
  void addMethod(const char *iface, const char *method, MethodHandler handler);
  void removeMethod(const char *iface, const char *method);

  bool hasInterface(const char *iface) const;
  bool hasMethod(const char *iface, const char *method) const;

  InterfaceHandlers getInterfaceHandlers() const;
  MethodHandlers getMethodHandlers(const char *iface) const;
  MethodHandler getMethodHandler(const char *iface, const char *method) const;

private:
  InterfaceHandlers mInterfaceHandlers;
  mutable std::mutex mHandlersMutex;
};