#include <ServerHandler.h>

namespace dashbus {

ServerHandler::ServerHandler() {
}

ServerHandler::~ServerHandler() {
  mInterfaceHandlers.clear();
}

void ServerHandler::addInterface(const char *iface, const MethodHandlers &handlers) {
  mInterfaceHandlers[iface] = handlers;
}

void ServerHandler::removeInterface(const char *iface) {
  mInterfaceHandlers.erase(iface);
}

void ServerHandler::addMethod(const char *iface, const char *method, MethodHandler handler) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface][method] = handler;
}

void ServerHandler::removeMethod(const char *iface, const char *method) {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  mInterfaceHandlers[iface].erase(method);
}

bool ServerHandler::hasInterface(const char *iface) const {
  return mInterfaceHandlers.count(iface) > 0;
}

bool ServerHandler::hasMethod(const char *iface, const char *method) const {
  std::lock_guard<std::mutex> lock(mHandlersMutex);
  if (hasInterface(iface)) {
    return mInterfaceHandlers.at(iface).count(method) > 0;
  }

  return false;
}

InterfaceHandlers ServerHandler::getInterfaceHandlers() const {
  return mInterfaceHandlers;
}

MethodHandlers ServerHandler::getMethodHandlers(const char *iface) const {
  return mInterfaceHandlers.at(iface);
}

MethodHandler ServerHandler::getMethodHandler(const char *iface, const char *method) const {
  return mInterfaceHandlers.at(iface).at(method);
}

} // namespace dashbus
