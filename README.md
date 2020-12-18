# ZenVar
## Overview
Mongoose OS library implementing ZenVar variant object.
## C/C++ API Reference
### enum mgos_zvar_type
```c
enum mgos_zvar_type {
  MGOS_ZVAR_TYPE_BOOL
  MGOS_ZVAR_TYPE_BIGINT
  MGOS_ZVAR_TYPE_DECIMAL
  MGOS_ZVAR_TYPE_STR
};
```
ZenVar data types.
### mgos_zvar_type()
```c
enum mgos_zvar_type mgos_zvar_type(mgos_zvar_t *var);
```
Returns the data type of the variant instance.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_equals()
```c
bool mgos_zvar_equals(mgos_zvar_t *var1, mgos_zvar_t *var2);
```
Compares two variants. Returns `true` if they are equal, otherwise `false`.

|Parameter||
|--|--|
|var1|Variant instance.|
|var2|Variant instance.|
### mgos_zvar_copy()
```c
mgos_zvar_t *mgos_zvar_copy(mgos_zvar_t *src, mgos_zvar_t *dest);
```
Copies the source variant to destination one.

|Parameter||
|--|--|
|src|Source variant instance.|
|dest|Destination variant instance.|
### mgos_zvar_nav_set()
```c
mgos_zvar_t *mgos_zvar_nav_set(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_is_nav()
```c
bool mgos_zvar_is_nav(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_bigint_set()
```c
mgos_zvar_t *mgos_zvar_bigint_set(mgos_zvar_t *var, long value);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_bool_set()
```c
mgos_zvar_t *mgos_zvar_bool_set(mgos_zvar_t *var, bool value);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_decimal_set()
```c
mgos_zvar_t *mgos_zvar_decimal_set(mgos_zvar_t *var, double value);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_str_set()
```c
mgos_zvar_t *mgos_zvar_str_set(mgos_zvar_t *var, const char *str);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|str|String to set.|
### mgos_zvar_bigint_get()
```c
long mgos_zvar_bigint_get(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_bool_get()
```c
bool mgos_zvar_bool_get(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_decimal_get()
```c
double mgos_zvar_decimal_get(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_str_get()
```c
const char *mgos_zvar_str_get(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### json_printf_zvar()
```c
int json_printf_zvar(struct json_out *out, va_list *ap);
```
*Description*

|Parameter||
|--|--|
|||
### mgos_zvar_free()
```c
void mgos_zvar_free(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_is_dic()
```c
bool mgos_zvar_is_dic(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_clear()
```c
void mgos_zvar_dic_clear(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_count()
```c
int mgos_zvar_dic_count(mgos_zvar_t *var);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_remove()
```c
void mgos_zvar_dic_remove(mgos_zvar_t *var, const char *name);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
### mgos_zvar_dic_bigint_get()
```c
long mgos_zvar_dic_bigint_get(mgos_zvar_t *var, const char *name);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
### mgos_zvar_dic_bool_get()
```c
bool mgos_zvar_dic_bool_get(mgos_zvar_t *var, const char *name);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
### mgos_zvar_dic_decimal_get()
```c
double mgos_zvar_dic_decimal_get(mgos_zvar_t *var, const char *name);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
### mgos_zvar_dic_str_get()
```c
const char *mgos_zvar_dic_str_get(mgos_zvar_t *var, const char *name);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
### mgos_zvar_dic_bigint_set()
```c
mgos_zvar_t *mgos_zvar_dic_bigint_set(mgos_zvar_t *var, const char *name, long val);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
|val|Value to set.|
### mgos_zvar_dic_bool_set()
```c
mgos_zvar_t *mgos_zvar_dic_bool_set(mgos_zvar_t *var, const char *name, bool val);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
|val|Value to set.|
### mgos_zvar_t *mgos_zvar_dic_decimal_set()
```c
mgos_zvar_t *mgos_zvar_dic_decimal_set(mgos_zvar_t *var, const char *name, double val);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
|val|Value to set.|
### mgos_zvar_dic_str_set()
```c
mgos_zvar_t *mgos_zvar_dic_str_set(mgos_zvar_t *var, const char *name, const char *str);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|name|Key name.|
|str|String to set.|
### mgos_zvar_dic_get_at()
```c
mgos_zvar_t *mgos_zvar_dic_get_at(mgos_zvar_t *var, int idx, const char **key);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index.|
|key|Key name (optional)|
### mgos_zvar_dic_bigint_get_at()
```c
long mgos_zvar_dic_bigint_get_at(mgos_zvar_t *var, int idx, const char **key);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index.|
|key|Key name (optional)|
### mgos_zvar_dic_bool_get_at()
```c
bool mgos_zvar_dic_bool_get_at(mgos_zvar_t *var, int idx, const char **key);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index.|
|key|Key name (optional)|
### mgos_zvar_dic_decimal_get_at()
```c
double mgos_zvar_dic_decimal_get_at(mgos_zvar_t *var, int idx, const char **key);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index.|
|key|Key name (optional)|
### mgos_zvar_dic_str_get_at()
```c
const char *mgos_zvar_dic_str_get_at(mgos_zvar_t *var, int idx, const char **key);
```
*Description*

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index.|
|key|Key name (optional)|
## JS API Reference
*Under construction...*
