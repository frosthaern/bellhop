#include <bellmsg.h>
#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int INITIAL_ACTION_SIZE = 10;

void print_msg(bellmsg_t *bell_msg) {
  printf("app_name: %s\n", bell_msg->app_name);
  printf("app_icon: %s\n", bell_msg->app_icon);
  printf("summary: %s\n", bell_msg->summary);
  printf("body: %s\n", bell_msg->body);
  printf("actions start:\n");
  for (int i = 0; i < bell_msg->actions->length; ++i) {
    printf("%d label = %s\n",i , bell_msg->actions->action_arr[i].label);
    printf("%d identifier = %s\n",i ,bell_msg->actions->action_arr[i].identifier);
  }
  printf("actions end:\n");
}

void handle_message(DBusMessage *msg) {
  if (!msg) return;

  DBusMessageIter iter;
  char *app_name = NULL;
  dbus_uint32_t replaces_id = 0;
  char *app_icon = NULL;
  char *summary = NULL;
  char *body = NULL;
  actions_t *actions = NULL;
  dbus_int32_t timeout = -1;

  dbus_message_iter_init(msg, &iter);

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, &app_name);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT32) {
    dbus_uint32_t id;
    dbus_message_iter_get_basic(&iter, &id);
    replaces_id = id;
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, &app_icon);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, &summary);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
    dbus_message_iter_get_basic(&iter, &body);
    dbus_message_iter_next(&iter);
  }

  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
    DBusMessageIter array_iter;
    dbus_message_iter_recurse(&iter, &array_iter);
    if (action_new(INITIAL_ACTION_SIZE, &actions) < 0) {
      printf("failed to run actions_new");
      exit(EXIT_FAILURE);
    }

    while (dbus_message_iter_get_arg_type(&array_iter) == DBUS_TYPE_STRING) {
      char *identifier = NULL;
      char *label = NULL;

      dbus_message_iter_get_basic(&array_iter, &identifier);
      dbus_message_iter_next(&array_iter);

      if (dbus_message_iter_get_arg_type(&array_iter) == DBUS_TYPE_STRING) {
        dbus_message_iter_get_basic(&array_iter, &label);
        dbus_message_iter_next(&array_iter);
      }

      if (action_append(actions, identifier, label) < 0) { printf("Failed to append action\n"); }
    }
    dbus_message_iter_next(&iter);

    /* this is the dictionary part, don't worry, do it tomorrow */
    /* i am actially skipping the dictionary part */

    if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
      // handle the dict stuff bro, it's kinda complicated, ask chatgpt
      dbus_message_iter_next(&iter);
    }
  }

  // Get timeout (int32)
  if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_INT32) { dbus_message_iter_get_basic(&iter, &timeout); }

  // Create and populate bellmsg_t
  bellmsg_t *bell_msg = malloc(sizeof(bellmsg_t));
  if (bell_msg) {
    bell_msg->app_name = app_name ? strdup(app_name) : NULL;
    bell_msg->replaces_id = replaces_id;
    bell_msg->app_icon = app_icon ? strdup(app_icon) : NULL;
    bell_msg->summary = summary ? strdup(summary) : NULL;
    bell_msg->body = body ? strdup(body) : NULL;
    bell_msg->actions = actions;
    bell_msg->timeout = timeout;
  }

  print_msg(bell_msg);
  
  // Cleanup
  if (bell_msg) {
    free(bell_msg->app_name);
    free(bell_msg->app_icon);
    free(bell_msg->summary);
    free(bell_msg->body);
    if (bell_msg->actions) action_free(bell_msg->actions);
    free(bell_msg);
  }
}

