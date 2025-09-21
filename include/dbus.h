#ifndef BELLHOP_DBUS_H
#define BELLHOP_DBUS_H

#include <dbus/dbus.h>

DBusError *dbus_init_error();
DBusConnection* dbus_init_connection(DBusError*);
static void handle_dbus_error(DBusError* error, const char* message);
int dbus_init_name(DBusConnection* conn, DBusError* error, const char* name);
int dbus_setup_notification_listener(DBusConnection* conn, DBusError* err);
int dbus_process_message(DBusConnection* conn);

#endif
