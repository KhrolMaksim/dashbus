#include <ServerHandler.h>

DashBus::ServerHandler::ServerHandler() {
}

DashBus::ServerHandler::~ServerHandler() {
  mInterfaceHandlers.clear();
}

void DashBus::ServerHandler::addInterface(const char *iface, const MethodHandlers &handlers) {
  mInterfaceHandlers[iface] = handlers;
}

void DashBus::ServerHandler::removeInterface(const char *iface) {
  mInterfaceHandlers.erase(iface);
}

void DashBus::ServerHandler::addMethod(const char *iface, const char *method,
                                       MethodHandler handler) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface][method] = handler;
}

void DashBus::ServerHandler::removeMethod(const char *iface, const char *method) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface].erase(method);
}

bool DashBus::ServerHandler::hasInterface(const char *iface) const {
  return mInterfaceHandlers.count(iface) > 0;
}

bool DashBus::ServerHandler::hasMethod(const char *iface, const char *method) const {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  if (hasInterface(iface)) {
    return mInterfaceHandlers.at(iface).count(method) > 0;
  }

  return false;
}

DashBus::InterfaceHandlers DashBus::ServerHandler::getInterfaceHandlers() const {
  return mInterfaceHandlers;
}

DashBus::MethodHandlers DashBus::ServerHandler::getMethodHandlers(const char *iface) const {
  return mInterfaceHandlers.at(iface);
}

DashBus::MethodHandler DashBus::ServerHandler::getMethodHandler(const char *iface,
                                                                const char *method) const {
  return mInterfaceHandlers.at(iface).at(method);
}
