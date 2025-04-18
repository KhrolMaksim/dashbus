#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Message.h>

class A {
public:
  auto getFieldsRef() {
    return std::tie(num, fNum, name, flag1, flag2);
  }
  auto getFieldsRef() const {
    return std::tie(num, fNum, name, flag1, flag2);
  }

  int num = 0;
  double fNum = 0.0;
  std::string name;
  bool flag1 = false;
  bool flag2 = false;
};

TEST_CASE("Message") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");

  DBusMessageIter writeIter;
  DBusMessageIter readIter;
  dbus_message_iter_init_append(msg.get(), &writeIter);

  SUBCASE("Добавление и получение int") {
    dashbus::Message::addMessageArgument(&writeIter, 42);

    dbus_message_iter_init(msg.get(), &readIter);
    int res;
    dashbus::Message::getMessageArgument(&readIter, res);
    CHECK(res == 42);
  }

  SUBCASE("Добавление и получение string") {
    dashbus::Message::addMessageArgument(&writeIter, std::string("hello"));

    dbus_message_iter_init(msg.get(), &readIter);
    std::string res;
    dashbus::Message::getMessageArgument(&readIter, res);
    CHECK(res == "hello");
  }

  SUBCASE("Добавление и получение класса A") {
    A a1;
    a1.num = 40;
    a1.fNum = 2.53;
    a1.name = "test name";
    a1.flag1 = false;
    a1.flag2 = true;

    dashbus::Message::addMessageArgument(&writeIter, a1);

    dbus_message_iter_init(msg.get(), &readIter);
    CHECK(dbus_message_iter_get_arg_type(&readIter) == DBUS_TYPE_STRUCT);

    A a2;
    dashbus::Message::getMessageArgument(&readIter, a2);
    CHECK(a1.num == a2.num);
    CHECK(a1.fNum == doctest::Approx(a2.fNum));
    CHECK(a1.name == a2.name);
    CHECK(a1.flag1 == a2.flag1);
    CHECK(a1.flag2 == a2.flag2);
  }
}