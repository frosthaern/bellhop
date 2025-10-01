#ifndef BELLMSG_H
#define BELLMSG_H

#include <action.h>
#include <dbus/dbus.h>
#include <hints.h>

typedef struct {
  char *app_name;
  dbus_uint32_t replaces_id;
  char *app_icon;
  char *summary;
  char *body;
  actions_t *actions;
  hint_t *hints;
  dbus_int32_t timeout;
} bellmsg_t;

#endif
