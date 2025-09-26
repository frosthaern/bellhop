#include "hints.h"
#include "dbus/dbus.h"
#include <stdint.h>
#include <stdlib.h>

/*
 * change the hint_type_t to the dbus enum that is already there
 * like pass the dbus enum itself and also just call the function inside the get_arg_ or whatever function call
 * and it will take this type and dbus_type enum and this function will take that automatically and and based on the case statements
 * call these functions
 */

hint_t * hints_new_hint_t(hint_type_t type, void *value, char *key) {
  hint_t *hint = malloc(sizeof(hint_t));
  hint->type = type;
  switch (type) {
  case HINT_TYPE_STRING: {
    hints_new_str_hint_t(hint, (char *)value, key);
    break;
  }

  case HINT_TYPE_UINT32: {
    hints_new_int_hint_t(hint, *(dbus_uint32_t *)value, key);
    break;
  }

  case HINT_TYPE_BOOLEAN: {
    hints_new_bool_hint_t(hint, *(dbus_bool_t *)value, key);
    break;
  }

  case HINT_TYPE_BYTE: {
    hints_new_byte_hint_t(hint, *(uint8_t *)value, key);
    break;
  }
  default: {
    /* return null because the type is invalid
       and i will have a null check while calling this function anyways */
    return NULL;
  }
  }
  return hint;
}

void hints_new_str_hint_t(hint_t *hint, char *value, char *key) {
  hint->key = key;
  hint->value.str_value = value;
}

void hints_new_int_hint_t(hint_t *hint, dbus_uint32_t value, char *key) {
  hint->key = key;
  hint->value.int_value = value;
}

void hints_new_bool_hint_t(hint_t *hint, dbus_bool_t value, char *key) {
  hint->key = key;
  hint->value.bool_value = value;
}

void hints_new_byte_hint_t(hint_t *hint, uint8_t value, char *key) {
  hint->key = key;
  hint->value.byte_value = value;
}
