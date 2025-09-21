#include <stdio.h>
#include <dbus/dbus.h>
#include <stdlib.h>
#include "dbus.h"

int main() {
  int ret;
  DBusError *err = dbus_init_error();
  DBusConnection* conn = dbus_init_connection(err);
  if ((ret = dbus_init_name(conn, err, "com.example.bellhop")) < 0) exit(EXIT_FAILURE);
  printf("Listening for DBus messages... (Press Ctrl+C to exit)\n");

}
