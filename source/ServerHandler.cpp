#include <ServerHandler.h>

dashbus::ServerHandler::ServerHandler() {
}

dashbus::ServerHandler::~ServerHandler() {
  mInterfaceHandlers.clear();
}

void dashbus::ServerHandler::addInterface(const char *iface, const MethodHandlers &handlers) {
  mInterfaceHandlers[iface] = handlers;
}

void dashbus::ServerHandler::removeInterface(const char *iface) {
  mInterfaceHandlers.erase(iface);
}

void dashbus::ServerHandler::addMethod(const char *iface, const char *method,
                                       MethodHandler handler) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface][method] = handler;
}

void dashbus::ServerHandler::removeMethod(const char *iface, const char *method) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface].erase(method);
}

bool dashbus::ServerHandler::hasInterface(const char *iface) const {
  return mInterfaceHandlers.count(iface) > 0;
}

bool dashbus::ServerHandler::hasMethod(const char *iface, const char *method) const {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  if (hasInterface(iface)) {
    return mInterfaceHandlers.at(iface).count(method) > 0;
  }

  return false;
}

dashbus::InterfaceHandlers dashbus::ServerHandler::getInterfaceHandlers() const {
  return mInterfaceHandlers;
}

dashbus::MethodHandlers dashbus::ServerHandler::getMethodHandlers(const char *iface) const {
  return mInterfaceHandlers.at(iface);
}

dashbus::MethodHandler dashbus::ServerHandler::getMethodHandler(const char *iface,
                                                                const char *method) const {
  return mInterfaceHandlers.at(iface).at(method);
}
