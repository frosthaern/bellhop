#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

int main() {
  DBusError error;
  dbus_error_init(&error);

  // first get connection =========================================================
  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
  
  if (!conn || dbus_error_is_set(&error)) {
    printf("Either failed to get connection or %s\n", error.message);
    exit(EXIT_FAILURE);
  }

  printf("Successfully procured a connection to dbus_session");
  
  // then register a name =======================================================
  char *device_name = "org.frosthaern.bellhop";
  int request_code = dbus_bus_request_name(conn, device_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

  if (dbus_error_is_set(&error)) {
    printf("%s\n", error.message);
  }
  
  if (request_code != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    printf("Not able to become primary owner\n");
    exit(EXIT_FAILURE);
  }

  printf("Successfully registered a device");
  
  // then establish match rules =================================================
  const char *match_rule = "type='signal', interface='org.freedesktop.Notifications'";
  dbus_bus_add_match(conn, match_rule, &error);

  if (dbus_error_is_set(&error)) {
    printf("The add match function errored with %s\n", error.message);
    dbus_connection_unref(conn);
    exit(EXIT_FAILURE);
  }

  printf("Match rule added successfully");
  
  // now in this main loop i need to wait for messages i guess ==================

  while () {
    
  }

  // cleanup ===================================================================
  dbus_connection_unref(conn);
  
}
