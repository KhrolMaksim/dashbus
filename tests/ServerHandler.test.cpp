#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <ServerHandler.h>

DBusHandlerResult mockHandler(DBusConnection *, DBusMessage *) {
  return DBUS_HANDLER_RESULT_HANDLED;
}

TEST_CASE("ServerHandler") {
  dashbus::ServerHandler handler;

  SUBCASE("Добавление и проверка интерфейса") {
    CHECK(not handler.hasInterface("com.example.Interface"));

    handler.addInterface("com.example.Interface", {});
    CHECK(handler.hasInterface("com.example.Interface"));
  }

  SUBCASE("Добавление и проверка метода") {
    CHECK(not handler.hasMethod("com.example.Interface", "Ping"));

    handler.addMethod("com.example.Interface", "Ping", mockHandler);
    CHECK(handler.hasMethod("com.example.Interface", "Ping"));
  }

  SUBCASE("Добавление и проверка интерфейса и метода") {
    CHECK(not handler.hasInterface("com.example.Interface"));
    CHECK(not handler.hasMethod("com.example.Interface", "Ping"));

    handler.addInterface("com.example.Interface", {});
    CHECK(handler.hasInterface("com.example.Interface"));
    CHECK(not handler.hasMethod("com.example.Interface", "Ping"));
    handler.addMethod("com.example.Interface", "Ping", mockHandler);
    CHECK(handler.hasMethod("com.example.Interface", "Ping"));
  }
}