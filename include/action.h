#ifndef ACTION_H
#define ACTION_H

#include <sys/types.h>

typedef struct {
  char *identifier;
  char *label;
} _action;

typedef struct {
  _action *action_arr;
  size_t capacity;
  size_t length;
} actions_t;

int action_append(actions_t *actions, char *identifier, char *label);
void action_free(actions_t *actions);
int action_new(const int size, actions_t **actions);

#endif
