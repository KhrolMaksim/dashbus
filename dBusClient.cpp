#include "dBusClient.h"

#include <iostream>

DBusCppClient::DBusCppClient(std::string name, std::string path, DBusBusType type)
    : mConnection(type), mName(name), mPath(path) {
  dbus_error_init(&mError);
}

DBusCppClient::~DBusCppClient() {
  dbus_error_free(&mError);
}

void DBusCppClient::sendMessage() {
  DBusCppMessage msg(mName.c_str(), mPath.c_str(), "org.example.MyInterface", "MyMethod");
  DBusCppReplyMessage reply(mConnection, msg);
}

DBusCppConnection &DBusCppClient::getConnection() {
  return mConnection;
}
