#include "action.h"
#include <stdlib.h>
#define _POSIX_C_SOURCE 200809L
#include <string.h>

int action_append(actions_t *actions, char *identifier, char *label) {
  if (actions == NULL || identifier == NULL || label == NULL) return -1;

  /* this only works if capacity or not originally 0 else 0 * 2 = 0 */
  
  if (actions->length == actions->capacity) {
    _action *action_arr = realloc(actions->action_arr, sizeof(_action) * (actions->capacity * 2));
    if (action_arr == NULL) return -1;
    actions->action_arr = action_arr;
    actions->capacity *= 2;
  }
  /* strdup returns intaa ticka */
  actions->action_arr[actions->length].identifier = strdup(identifier);
  actions->action_arr[actions->length].label = strdup(label);
  actions->length++;
  return 0;
}

void action_free(actions_t *actions) {
  for (int i = 0; i < actions->length; ++i) {
    free(actions->action_arr[i].identifier);
    free(actions->action_arr[i].label);
  }
  free(actions->action_arr);
  free(actions);
  actions = NULL;
}

int action_new(const int size, actions_t **actions) {
  *actions = malloc(sizeof(actions_t));
  if (actions == NULL) return -1;
  (*actions)->capacity = size;
  (*actions)->length = 0;
  (*actions)->action_arr = malloc(sizeof(_action) * size);
  if ((*actions)->action_arr == NULL) return -1;
  return 0;
}
