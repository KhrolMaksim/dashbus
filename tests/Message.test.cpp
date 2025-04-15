#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Message.h>

TEST_CASE("Message") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");

  SUBCASE("Добавление и получение int") {
    msg.appendArgument(42);
    CHECK(msg.getArgument<int>() == 42);
  }

  SUBCASE("Добавление и получение string") {
    msg.appendArgument(std::string("hello"));
    CHECK(msg.getArgument<std::string>() == "hello");
  }

  // Возникает ошибка времени компиляции т.к. нет явного инстанци́рования
  // SUBCASE("Unsupported type throws") {
  //   CHECK_THROWS_AS(msg.appendArgument(3.14), dashbus::NameRequestException);
  // }
}