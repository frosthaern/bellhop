#include "dbus.h"
#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

DBusError* dbus_init_error() {
  DBusError* err;
  dbus_error_init(err);
  return err;
}

DBusConnection* dbus_init_conn(DBusError* error) {
  DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, error);
  if (dbus_error_is_set(error)) handle_dbus_error(error, "Failed to connect to session bus");
  return conn;
}

static void handle_dbus_error(DBusError* error, const char* message) {
  fprintf(stderr, "%s: %s\n", message, error->message);
  dbus_error_free(error);
  exit(EXIT_FAILURE);
}

int dbus_init_name(DBusConnection* conn, DBusError* error, const char* name) {
  int ret = dbus_bus_request_name(conn, name, DBUS_NAME_FLAG_REPLACE_EXISTING, error);
  if (dbus_error_is_set(error)) handle_dbus_error(error, "not able to request a name in dbus");
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    fprintf(stderr, "Not the primary owner of the name %s", name);
    return -1;
  }
  return 1;
}

int dbus_setup_notification_listener(DBusConnection* conn, DBusError* err) {
  if (!conn) {
    fprintf(stderr, "Invalid Connection\n");
    return -1;
  }

  const char* match_rule = "type='method_call',"
                           "interface='org.freedesktop.Notification',"
                           "member='Notify',";
  dbus_bus_add_match(conn, match_rule, err);
  if (dbus_error_is_set(err)) {
    handle_dbus_error(err, "Failed to add match rule");
    return -1;
  }

  dbus_connection_flush(conn);
  printf("Notification Listener setup successfully\n");
  return 1;
}

/*
 * TODO: things to do below
 * we have to write a few functions that will take string or uint32 or whatnot, it will do iter and get that value
 * i will write the functions for array and dict too because i need actions and key: value and whatnot
 * so i will use that, and get all the values and print it
 * this is what i am supposed to do next time, i visit this codebase
 */
