#include "bellmsg.h"
#include "bellhop_message.h"
#include "dbus/dbus-protocol.h"
#include <dbus/dbus.h>
#include <stdlib.h>

#define INITIAL_ACTION_SIZE 10

void handle_message(DBusMessage *msg) {

  char *app_name = NULL;
  dbus_uint32_t replaces_id;
  char *app_icon = NULL;
  char *summary = NULL;
  char *body = NULL;
  actions_t *actions = malloc(sizeof(actions_t));
  actions->action_arr = malloc(sizeof(_action) * INITIAL_ACTION_SIZE);
  actions->capacity = INITIAL_ACTION_SIZE;
  actions->length = 0;
  dbus_uint32_t timeout;

  DBusMessageIter iter, array_iter, dict_iter, variant_iter;

  dbus_message_iter_init(msg, &iter);

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, app_name);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT32) {
    dbus_message_iter_get_basic(&iter, replaces_id);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, app_icon);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, summary);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, body);
    dbus_message_iter_next(&iter);
  }

  
  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
    dbus_message_iter_recurse(&iter, &array_iter);
    _action *action = malloc(sizeof(_action));
    while (dbus_message_iter_get_arg_type(&array_iter) == DBUS_TYPE_STRING) {
      dbus_message_iter_get_basic(&array_iter, action->identifier);
      dbus_message_iter_next(&array_iter);
      if (dbus_message_iter_get_arg_type(&array_iter) == DBUS_TYPE_STRING) {
        dbus_message_iter_get_basic(&array_iter, action->label);
	dbus_message_iter_next(&array_iter);
      }
      if (action_append(actions, action) < 0) {
        printf("Not able to append actions for some reason");
	exit(EXIT_FAILURE);
      }
    }
    dbus_message_iter_next(&iter);

    /* this is the dictionary part, don't worry, do it tomorrow */

    if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
      // handle the dict stuff bro, it's kinda complicated, ask chatgpt
      dbus_message_iter_next(&iter);
    }

    if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT32) {
      dbus_message_iter_get_basic(&iter, timeout);
      dbus_message_iter_next(&iter);
    }
    
  }
}
