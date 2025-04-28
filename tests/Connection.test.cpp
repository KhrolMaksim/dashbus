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

TEST_CASE("Copy test") {
  SUBCASE("Одиночное копирование") {
    dashbus::Connection first(DBUS_BUS_SESSION);
    dashbus::Connection second = dashbus::Connection::createByPointer(first.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
  }

  SUBCASE("Двойное копирование") {
    dashbus::Connection first(DBUS_BUS_SESSION);
    dashbus::Connection second = dashbus::Connection::createByPointer(first.get());
    dashbus::Connection thread = dashbus::Connection::createByPointer(first.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
    CHECK(thread.get() != nullptr);
  }

  SUBCASE("Двойное каскадное") {
    dashbus::Connection first(DBUS_BUS_SESSION);
    dashbus::Connection second = dashbus::Connection::createByPointer(first.get());
    dashbus::Connection thread = dashbus::Connection::createByPointer(second.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
    CHECK(thread.get() != nullptr);
  }

  SUBCASE("Массивное") {
    dashbus::Connection с0(DBUS_BUS_SESSION);
    dashbus::Connection с1 = dashbus::Connection::createByPointer(с0.get());
    dashbus::Connection с2 = dashbus::Connection::createByPointer(с1.get());
    dashbus::Connection с3 = dashbus::Connection::createByPointer(с1.get());
    dashbus::Connection с4 = dashbus::Connection::createByPointer(с3.get());
    dashbus::Connection с5 = dashbus::Connection::createByPointer(с3.get());
    dashbus::Connection с6 = dashbus::Connection::createByPointer(с4.get());
    dashbus::Connection с7 = dashbus::Connection::createByPointer(с0.get());
    dashbus::Connection с8 = dashbus::Connection::createByPointer(с2.get());
    dashbus::Connection с9 = dashbus::Connection::createByPointer(с8.get());
    dashbus::Connection с10 = dashbus::Connection::createByPointer(с3.get());
    dashbus::Connection с11 = dashbus::Connection::createByPointer(с0.get());

    CHECK(с0.get() != nullptr);
    CHECK(с1.get() != nullptr);
    CHECK(с2.get() != nullptr);
    CHECK(с3.get() != nullptr);
    CHECK(с4.get() != nullptr);
    CHECK(с5.get() != nullptr);
    CHECK(с6.get() != nullptr);
    CHECK(с7.get() != nullptr);
    CHECK(с8.get() != nullptr);
    CHECK(с9.get() != nullptr);
    CHECK(с10.get() != nullptr);
    CHECK(с11.get() != nullptr);
  }
}