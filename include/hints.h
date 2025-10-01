#ifndef HINTS_H
#define HINTS_H

#include <dbus/dbus.h>
#include <stdint.h>

typedef enum {
  HINT_TYPE_STRING,
  HINT_TYPE_UINT32,
  HINT_TYPE_BOOLEAN,
  HINT_TYPE_BYTE,
} hint_type_t;

typedef union {
  char *str_value;
  int32_t int_value;
  uint8_t byte_value;
  dbus_bool_t bool_value;
} hint_value_union;

typedef struct {
  hint_type_t type;
  char *key;
  hint_value_union value;
} hint_t;

hint_t *hints_new_hint_t(hint_type_t type, void *value, char *key);
void hints_new_str_hint_t(hint_t *hint, char *value, char *key);
void hints_new_int_hint_t(hint_t *hint, dbus_uint32_t value, char *key);
void hints_new_bool_hint_t(hint_t *hint, dbus_bool_t value, char *key);
void hints_new_byte_hint_t(hint_t *hint, uint8_t value, char *key);

#endif
