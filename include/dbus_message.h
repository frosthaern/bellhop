#ifndef DBUS_MESSAGE_H
#define DBUS_MESSAGE_H

#include "dbus/dbus.h"
#include <dbus.h>

typedef struct {
  char *identifier;
  char *label;
} action;

typedef struct {
  char *app_name;
  dbus_uint32_t replaces_id;
  char *app_icon;
  char *summary;
  char *body;
  action *actions;
  hint_t *hints;
  dbus_int32_t timeout;
} dbus_msg_t;

#endif
