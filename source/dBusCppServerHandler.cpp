#include <dBusCppServerHandler.h>

DBusCppServerHandler::DBusCppServerHandler() {
}

DBusCppServerHandler::~DBusCppServerHandler() {
  mInterfaceHandlers.clear();
}

void DBusCppServerHandler::addInterface(const char *iface, const MethodHandlers &handlers) {
  mInterfaceHandlers[iface] = handlers;
}

void DBusCppServerHandler::removeInterface(const char *iface) {
  mInterfaceHandlers.erase(iface);
}

void DBusCppServerHandler::addMethod(const char *iface, const char *method, MethodHandler handler) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface][method] = handler;
}

void DBusCppServerHandler::removeMethod(const char *iface, const char *method) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface].erase(method);
}

bool DBusCppServerHandler::hasInterface(const char *iface) const {
  return mInterfaceHandlers.count(iface) > 0;
}

bool DBusCppServerHandler::hasMethod(const char *iface, const char *method) const {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  if (hasInterface(iface)) {
    return mInterfaceHandlers.at(iface).count(method) > 0;
  }

  return false;
}

InterfaceHandlers DBusCppServerHandler::getInterfaceHandlers() const {
  return mInterfaceHandlers;
}

MethodHandlers DBusCppServerHandler::getMethodHandlers(const char *iface) const {
  return mInterfaceHandlers.at(iface);
}

MethodHandler DBusCppServerHandler::getMethodHandler(const char *iface, const char *method) const {
  return mInterfaceHandlers.at(iface).at(method);
}
