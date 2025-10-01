#include "dbus/dbus-protocol.h"
#include "dbus/dbus-shared.h"
#include <bellmsg.h>
#include <dbus/dbus-message.h>
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

  /* ========================== register a name ========================== */

  if (dbus_bus_request_name(conn, "org.freedesktop.Notifications", DBUS_NAME_FLAG_REPLACE_EXISTING, &error) != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    printf("the process was not able to become the primary owner because {%s}", error.message);
    exit(EXIT_FAILURE);
  }
  
  printf("the process became the primary owner because {%s}", error.message);

  /* ========================== then establish match rules for mathod_call ========================== */

  const char *match_rule = "type='method_call', interface='org.freedesktop.Notifications', member=Notify";
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
      DBusMessage *reply = dbus_message_new_method_return(msg);
      if (!reply) {
        printf("Failed to create reply message\n");
	return EXIT_FAILURE;
      }

      dbus_uint32_t notification_id = 0;
      dbus_message_append_args(reply, DBUS_TYPE_UINT32, &notification_id, DBUS_TYPE_INVALID);

      dbus_connection_send(conn, reply, NULL);
      dbus_connection_flush(conn);

    } else {
      printf("Got some other message\n");
    }

    dbus_message_unref(msg);
  }

  dbus_connection_unref(conn);
}
