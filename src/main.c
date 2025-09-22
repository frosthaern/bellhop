#include <stdio.h>
#include <dbus/dbus.h>
#include <stdlib.h>
#include <unistd.h>
#include "dbus.h"

int main() {
  int ret;
  DBusError err;
  dbus_error_init(&err);
  DBusConnection* conn = dbus_init_conn(&err);
  if ((ret = dbus_init_name(conn, &err, "com.example.bellhop")) < 0) exit(EXIT_FAILURE);
  printf("Listening for DBus messages... (Press Ctrl+C to exit)\n");

  int count = 0;
  while (dbus_process_message(conn) >= 0) {
    printf("============================================================");
    printf("this is the %d'th message", count);
    printf("============================================================");
    sleep(100);
  }
}
