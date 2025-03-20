#include "dBusClient.h"

int main() {
  DBusCppClient client("org.example.MyInterface", "/org/example/MyObject", DBUS_BUS_SESSION);
  client.sendMessage();

  return 0;
}
