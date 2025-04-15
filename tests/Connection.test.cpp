#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Connection.h>

TEST_CASE("Connection") {
  SUBCASE("Подключение к dbus (system)") {
    dashbus::Connection conn(DBUS_BUS_SYSTEM);
    CHECK(conn.get() != nullptr);
  }

  SUBCASE("Подключение к dbus (session)") {
    dashbus::Connection conn(DBUS_BUS_SESSION);
    CHECK(conn.get() != nullptr);
  }

  SUBCASE("Проверка создания с корректными типами") {
    CHECK_NOTHROW(dashbus::Connection(static_cast<DBusBusType>(DBUS_BUS_SESSION)));
    CHECK_NOTHROW(dashbus::Connection(static_cast<DBusBusType>(DBUS_BUS_SYSTEM)));
    CHECK_NOTHROW(dashbus::Connection(static_cast<DBusBusType>(DBUS_BUS_STARTER)));
  }
}