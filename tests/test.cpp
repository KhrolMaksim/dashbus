#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("namespace 1 global operator") {
  int a = 1;
  int b = 1;
  CHECK(a == b);
}