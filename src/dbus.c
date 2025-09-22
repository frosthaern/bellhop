#include "dbus.h"
#include <dbus/dbus.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum {
  HINT_TYPE_STRING,
  HINT_TYPE_UINT32,
  HINT_TYPE_BOOLEAN,
  HINT_TYPE_BYTE,
} hint_value_type;

typedef struct {
  hint_value_type type;
  union {
    char* str;
    int32_t int32;
    uint8_t byte;
    dbus_bool_t bool;
  } value;
} hint_value;

typedef struct {
  char* key;
  hint_value value;
} hint_entry;


typedef struct {
  const char* app_name;
  uint32_t replaces_it;
  const char* app_icon;
  const char* summary;
  const char* body;
  const char** actions;
  hint_entry hint;
} notify_message;


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

int dbus_process_message(DBusConnection* conn) {
  DBusMessage *msg;
  if (!conn) return -1;

  dbus_connection_read_write(conn, 0);

  msg = dbus_connection_pop_message(conn);
  if (!msg) return 0;

  DBusMessageIter iter;
  if (!dbus_message_iter_init(msg, &iter)) {
    return -1;
  }

  const char* app_name = get_string_from_iter(&iter);
  uint32_t replaces_id = get_uint32_from_iter(&iter);
  const char* app_icon = get_string_from_iter(&iter);
  const char* summary = get_string_from_iter(&iter);
  const char* body = get_string_from_iter(&iter);

  size_t actions_count = 0;
  char **actions = get_string_array_from_iter(&iter, &actions_count);

  return 1;
}

char* get_string_from_iter(DBusMessageIter* iter) {
  char *value;
  if (dbus_message_iter_get_arg_type(iter) != DBUS_TYPE_STRING) {
    fprintf(stderr, "Tried to extract a string type where the type was not string using iter");
    exit(1);
  }
  dbus_message_iter_get_basic(iter, &value);
  dbus_message_iter_next(iter);
  return value;
}

dbus_uint32_t get_uint32_from_iter(DBusMessageIter* iter) {
  uint32_t value;
  if (dbus_message_iter_get_arg_type(iter) != DBUS_TYPE_UINT32) {
    fprintf(stderr, "Tried to extract a string type where the type was not string using iter");
    exit(1);
  }
  dbus_message_iter_get_basic(iter, &value);
  dbus_message_iter_next(iter);
  return value;
}

char** get_string_array_from_iter(DBusMessageIter* iter, size_t *out_count) {
  if (dbus_message_iter_get_arg_type(iter) != DBUS_TYPE_ARRAY) {
    fprintf(stderr, "Tried to extract a string type where the type was not string using iter");
    exit(1);
  }

  DBusMessageIter sub_iter;
  dbus_message_iter_recurse(iter, &sub_iter);
  
  char **value = malloc(sizeof(char*) * 100);
  size_t count = 0;
  while (dbus_message_iter_get_arg_type(&sub_iter) == DBUS_TYPE_STRING) {
    char *read_string;
    dbus_message_iter_get_basic(&sub_iter, read_string);
    value[count++] = read_string;
    dbus_message_iter_next(&sub_iter);
  }

  out_count = count;
  return value;
}


