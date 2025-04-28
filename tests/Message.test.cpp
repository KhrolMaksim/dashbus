#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Message.h>

class A {
public:
  auto getFieldsRef() {
    return std::tie(num, fNum, name, vec, flag1, flag2);
  }
  auto getFieldsRef() const {
    return std::tie(num, fNum, name, vec, flag1, flag2);
  }

  int num = 0;
  double fNum = 0.0;
  std::string name;
  std::vector<int> vec;
  bool flag1 = false;
  bool flag2 = false;
};

class B {
public:
  auto getFieldsRef() {
    return std::tie(num, vec1, name, vec2, flag1, flag2);
  }
  auto getFieldsRef() const {
    return std::tie(num, vec1, name, vec2, flag1, flag2);
  }

  int num = 0;
  std::vector<A> vec1;
  std::string name;
  std::vector<A> vec2;
  bool flag1 = false;
  bool flag2 = false;
};

TEST_CASE("Добавление и получение int") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");
  msg.startWriteArguments();

  msg.addArgument(42);

  int res;
  msg.startReadArguments();
  msg.getArgument(res);

  CHECK(res == 42);
}

TEST_CASE("Добавление и получение string") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");
  msg.startWriteArguments();

  msg.addArgument(std::string("hello"));

  std::string res;
  msg.startReadArguments();
  msg.getArgument(res);

  CHECK(res == "hello");
}

TEST_CASE("Добавление и получение класса A") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");
  msg.startWriteArguments();

  A a1;
  a1.num = 40;
  a1.fNum = 2.53;
  a1.name = "test name";
  a1.vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  a1.flag1 = false;
  a1.flag2 = true;

  msg.addArgument(a1);

  msg.startReadArguments();
  CHECK(msg.getArgumentType() == DBUS_TYPE_STRUCT);

  A a2;
  msg.getArgument(a2);

  CHECK(a1.num == a2.num);
  CHECK(a1.fNum == doctest::Approx(a2.fNum));
  CHECK(a1.name == a2.name);
  CHECK(a1.vec.size() == a2.vec.size());
  size_t limit = std::min(a1.vec.size(), a2.vec.size());
  for (size_t i = 0; i < limit; i++) {
    CHECK(a1.vec[i] == a2.vec[i]);
  }
  CHECK(a1.flag1 == a2.flag1);
  CHECK(a1.flag2 == a2.flag2);
}

TEST_CASE("Добавление и получение класса B") {
  dashbus::Message msg("com.example", "/path", "com.example.Interface", "Method");
  msg.startWriteArguments();

  B b1;

  b1.num = 4;
  b1.name = "Hello difficult struct";
  b1.flag1 = true;
  b1.flag2 = false;

  for (size_t i; i < 10; i++) {
    A a;
    a.num = 30 + i;
    a.fNum = 2.53 + 0.2 * i;
    a.name = "test name 1";
    for (int j = 0; j < 10; j++) {
      a.vec.push_back(7 + i * 10 + j);
    }
    a.flag1 = false;
    a.flag2 = true;

    b1.vec1.push_back(a);
  }

  for (size_t i; i < 10; i++) {
    A a;
    a.num = 51 + i;
    a.fNum = 2.513 + 0.2 * i;
    a.name = "test name 2";
    for (int j = 0; j < 10; j++) {
      a.vec.push_back(95 + i * 10 + j);
    }
    a.flag1 = false;
    a.flag2 = true;

    b1.vec2.push_back(a);
  }

  msg.addArgument(b1);

  msg.startReadArguments();
  CHECK(msg.getArgumentType() == DBUS_TYPE_STRUCT);

  B b2;
  msg.getArgument(b2);

  CHECK(b1.num == b2.num);
  CHECK(b1.name == b2.name);

  CHECK(b1.vec1.size() == b2.vec1.size());
  size_t limit1 = std::min(b1.vec1.size(), b2.vec1.size());
  for (size_t i = 0; i < limit1; i++) {
    // CHECK(b1.vec1[i] == b2.vec1[i]);
    A &a1 = b1.vec1[i];
    A &a2 = b2.vec2[i];
    CHECK(a1.num == a2.num);
    CHECK(a1.fNum == doctest::Approx(a2.fNum));
    CHECK(a1.name == a2.name);
    CHECK(a1.vec.size() == a2.vec.size());
    size_t limit = std::min(a1.vec.size(), a2.vec.size());
    for (size_t i = 0; i < limit; i++) {
      CHECK(a1.vec[i] == a2.vec[i]);
    }
    CHECK(a1.flag1 == a2.flag1);
    CHECK(a1.flag2 == a2.flag2);
  }

  CHECK(b1.vec2.size() == b2.vec2.size());
  size_t limit2 = std::min(b1.vec2.size(), b2.vec2.size());
  for (size_t i = 0; i < limit2; i++) {
    // CHECK(b1.vec2[i] == b2.vec2[i]);
    A &a1 = b1.vec1[i];
    A &a2 = b2.vec2[i];
    CHECK(a1.num == a2.num);
    CHECK(a1.fNum == doctest::Approx(a2.fNum));
    CHECK(a1.name == a2.name);
    CHECK(a1.vec.size() == a2.vec.size());
    size_t limit = std::min(a1.vec.size(), a2.vec.size());
    for (size_t i = 0; i < limit; i++) {
      CHECK(a1.vec[i] == a2.vec[i]);
    }
    CHECK(a1.flag1 == a2.flag1);
    CHECK(a1.flag2 == a2.flag2);
  }

  CHECK(b1.flag1 == b2.flag1);
  CHECK(b1.flag2 == b2.flag2);
}

TEST_CASE("Copy test") {
  SUBCASE("Одиночное копирование") {
    dashbus::Message first("service_name.test", "/path", "interface.test", "method");
    dashbus::Message second = dashbus::Message::createByPointer(first.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
  }

  SUBCASE("Двойное копирование") {
    dashbus::Message first("service_name.test", "/path", "interface.test", "method");
    dashbus::Message second = dashbus::Message::createByPointer(first.get());
    dashbus::Message thread = dashbus::Message::createByPointer(first.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
    CHECK(thread.get() != nullptr);
  }

  SUBCASE("Двойное каскадное") {
    dashbus::Message first("service_name.test", "/path", "interface.test", "method");
    dashbus::Message second = dashbus::Message::createByPointer(first.get());
    dashbus::Message thread = dashbus::Message::createByPointer(second.get());

    CHECK(first.get() != nullptr);
    CHECK(second.get() != nullptr);
    CHECK(thread.get() != nullptr);
  }

  SUBCASE("Массивное") {
    dashbus::Message m0("service_name.test", "/path", "interface.test", "method");
    dashbus::Message m1 = dashbus::Message::createByPointer(m0.get());
    dashbus::Message m2 = dashbus::Message::createByPointer(m1.get());
    dashbus::Message m3 = dashbus::Message::createByPointer(m1.get());
    dashbus::Message m4 = dashbus::Message::createByPointer(m3.get());
    dashbus::Message m5 = dashbus::Message::createByPointer(m3.get());
    dashbus::Message m6 = dashbus::Message::createByPointer(m4.get());
    dashbus::Message m7 = dashbus::Message::createByPointer(m0.get());
    dashbus::Message m8 = dashbus::Message::createByPointer(m2.get());
    dashbus::Message m9 = dashbus::Message::createByPointer(m8.get());
    dashbus::Message m10 = dashbus::Message::createByPointer(m3.get());
    dashbus::Message m11 = dashbus::Message::createByPointer(m0.get());

    CHECK(m0.get() != nullptr);
    CHECK(m1.get() != nullptr);
    CHECK(m2.get() != nullptr);
    CHECK(m3.get() != nullptr);
    CHECK(m4.get() != nullptr);
    CHECK(m5.get() != nullptr);
    CHECK(m6.get() != nullptr);
    CHECK(m7.get() != nullptr);
    CHECK(m8.get() != nullptr);
    CHECK(m9.get() != nullptr);
    CHECK(m10.get() != nullptr);
    CHECK(m11.get() != nullptr);
  }
}