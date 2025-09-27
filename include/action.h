#ifndef ACTION_H
#define ACTION_H

typedef struct {
  char *identifier;
  char *label;
} _action;

typedef struct {
  _action *action_arr;
  size_t capacity;
  size_t length;
} actions_t;

int action_append(actions_t *actions, _action *action);

#endif
