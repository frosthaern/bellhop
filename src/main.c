#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  DBusError error;
  dbus_error_init(&error);

  /* ============================ first get connection ============================= */
  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &error);

  if (!conn || dbus_error_is_set(&error)) {
    printf("Either failed to get connection or %s\n", error.message);
    exit(EXIT_FAILURE);
  }

  printf("Successfully procured a connection to dbus_session\n");

  /* ========================== then establish match rules for mathod_call ========================== */
  const char *match_rule = "type='method_call', interface='org.freedesktop.Notifications', member=Notify, eavesdrop=true";
  dbus_bus_add_match(conn, match_rule, &error);

  if (dbus_error_is_set(&error)) {
    printf("The add match function errored with %s\n", error.message);
    dbus_connection_unref(conn);
    exit(EXIT_FAILURE);
  }

  printf("Added eavesdrop match for notify calls\n");

  /* ========================== now in this main loop i need to wait for messages i guess ========================= */

  printf("Listening to notification");

  while (1) {
    dbus_connection_read_write(conn, -1);
    DBusMessage *msg = dbus_connection_pop_message(conn);

    if (msg == NULL) { continue; }

    if (dbus_message_is_signal(msg, "org.freedesktop.Notifications", "NotificationClosed")) {
      printf("NotificationClosed signal received\n");
    } else if (dbus_message_is_signal(msg, "org.freedesktop.Notifications", "ActionInvoked")) {
      printf("ActionInvoked signal received\n");
    } else if (dbus_message_is_method_call(msg, "org.freedesktop.Notifications", "Notify") ||
               dbus_message_is_method_call(msg, "org.freedesktop.Notifications", "GetCapabilities") ||
               dbus_message_is_method_call(msg, "org.freedesktop.Notifications", "GetServerInformation")) {

      printf("Notify method call received\n");

      handle_message(msg);

    } else {
      printf("Got some other message\n");
    }

    dbus_message_unref(msg);
  }

  /* ========================== cleanup code ========================= */
  dbus_connection_unref(conn);
}
