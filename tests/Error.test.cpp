#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Error.h>

TEST_CASE("Error") {
  dashbus::Error err;

  SUBCASE("Старт со сброшенного значения") {
    CHECK_FALSE(err.isSet());
  }

  SUBCASE("Установка ошибки") {
    CHECK(not err.isSet());
    CHECK(err.name() == "");
    err.set("com.example.Error", "Something went wrong");
    CHECK(err.isSet());
    CHECK(err.name() == "com.example.Error");
  }

  SUBCASE("Бросок ошибки") {
    CHECK_NOTHROW(err.throwIfSet());
    err.set("test", "error");
    CHECK_THROWS_AS(err.throwIfSet(), dashbus::NameRequestException);
  }
}