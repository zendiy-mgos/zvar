# ZenVar
## Overview
Mongoose OS library implementing variant variables (like `var` statement in javascript). Using this library you can create variables which haven't data type declared explicitly, but any one of the followings:   
* Boolean (`bool`)
* Integer (`long`)
* Decimal (`double`)
* String (`char *`)
* Dictionary (key/value pair dictionary) - It requires msog_zvar_dic libray.

```c
// `NULL` variable initialization (like void* var = `NULL`)
mgos_zvar_t var = mgos_zvar_new();

// Integer variable initialization (like long var = 101;)
mgos_zvar_t var = mgos_zvar_new_integer(101);

// Boolean variable initialization (like bool var = true;)
mgos_zvar_t var = mgos_zvar_new_bool(true);

// Decimal variable initialization (like double var = 101.99;)
mgos_zvar_t var = mgos_zvar_new_decimal(101.99);

// String variable initialization (like char *var = "Lorem Ipsum";)
mgos_zvar_t var = mgos_zvar_new_str("Lorem Ipsum");
```
## C/C++ API Reference
### enum mgos_zvar_type
```c
enum mgos_zvar_type {
  MGOS_ZVAR_TYPE_NULL,
  MGOS_ZVAR_TYPE_BOOL,
  MGOS_ZVAR_TYPE_INTEGER,
  MGOS_ZVAR_TYPE_DECIMAL,
  MGOS_ZVAR_TYPE_STR
};
```
ZenVar variant data-types.
### mgos_zvar_get_type()
```c
enum mgos_zvar_type mgos_zvar_get_type(mgos_zvarc_t var);
```
Returns the variable [data-type](https://github.com/zendiy-mgos/zvar#enum-mgos_zvar_get_type).

|Parameter||
|--|--|
|var|Variant variable|
### mgos_zvar_new()
```c
mgos_zvar_t mgos_zvar_new();
```
Creates a variable and initializes it to `NULL`, with no data-type defined. Returns `NULL` in case of error. 
### mgos_zvar_new_integer(), mgos_zvar_new_bool(), mgos_zvar_new_decimal() and mgos_zvar_new_str()
```c       
mgos_zvar_t mgos_zvar_new_integer(long value);
mgos_zvar_t mgos_zvar_new_bool(bool value);
mgos_zvar_t mgos_zvar_new_decimal(double value);
mgos_zvar_t mgos_zvar_new_str(const char *value);
```
Creates and initializes a variable. Returns `NULL` in case of error. Invoking `mgos_zvar_new_str(NULL)` is equivalent to `mgos_zvar_new()`.

|Parameter||
|--|--|
|value|Value to be set.|
### mgos_zvar_set_null()
```c 
void mgos_zvar_set_null(mgos_zvar_t var);
```
Sets the variable value to `NULL`.

|Parameter||
|--|--|
|var|Variant variable.|
### mgos_zvar_set_integer(), mgos_zvar_set_bool(), mgos_zvar_set_decimal() and mgos_zvar_set_str()
```c                                 
void mgos_zvar_set_integer(mgos_zvar_t var, long value);
void mgos_zvar_set_bool(mgos_zvar_t var, bool value);
void mgos_zvar_set_decimal(mgos_zvar_t var, double value);
void mgos_zvar_set_str(mgos_zvar_t var, const char *value);
```
Sets the variable value. Invoking `mgos_zvar_set_str(var, NULL)` is equivalent to `mgos_zvar_set_null(var)`.

|Parameter||
|--|--|
|var|Variant variable.|
|value|Value to be set.|
### mgos_zvar_set_nstr()
```c 
void mgos_zvar_set_nstr(mgos_zvar_t var, const char *value, size_t value_len);
```
Sets the variable value. This is a specialized version of `mgos_zvar_set_str` Invoking `mgos_zvar_set_nstr(var, NULL, <any_value>)` is equivalent to `mgos_zvar_set_null(var)`.

|Parameter||
|--|--|
|var|Variant variable.|
|value|String value to set.|
|value_len|Maximum number of characters to be set. Ignored if the passed `value` parameter is `NULL`.|
### mgos_zvar_get_integer(), mgos_zvar_get_bool(), mgos_zvar_get_decimal() and mgos_zvar_get_str()
```c 
long mgos_zvar_get_integer(mgos_zvarc_t var);
bool mgos_zvar_get_bool(mgos_zvarc_t var);
double mgos_zvar_get_decimal(mgos_zvarc_t var);
const char *mgos_zvar_get_str(mgos_zvarc_t var);
```
Returns the variable value.

|Parameter||
|--|--|
|var|Variant variable.|

**Remarks**

The returned value depends on the input variable data-type. Please refer to details below.
|Function / Input|INTEGER|BOOL|DECIMAL|STRING|Any other|
|--|--|--|--|--|--|
|mgos_zvar_get_integer|Returns the integer value|Returns `0` if input value is `false`|Returns the integer part of the decimal|Returns `0`|Returns `0`|
|mgos_zvar_get_bool|Returns `false` if input value is `0`|Returns the boolean value|Returns `false` if input value is `0.0`|Returns `false` if input string is empty|Returns `false`|
|mgos_zvar_get_decimal|Returns the input value as decimal|Returns `0.0`|Returns the decimal value|Returns `0.0`|Returns `0.0`|
|mgos_zvar_get_str|Returns `NULL`|Returns `NULL`|Returns `NULL`|Returns the string value|Returns `NULL`|
### mgos_zvar_is_equal()
```c
bool mgos_zvar_is_equal(mgos_zvarc_t var1, mgos_zvarc_t var2);
```
Compares two variables. Returns `true` if they are equal, otherwise `false`.

|Parameter||
|--|--|
|var1|Variant variable.|
|var2|Variant variable.| 
### mgos_zvar_is_null()
```c
bool mgos_zvar_is_null(mgos_zvarc_t var);
```
Returns `true` if the variable value is `NULL`, otherwise `false`.

|Parameter||
|--|--|
|var|Variant variable.|
### mgos_zvar_copy()
```c
bool mgos_zvar_copy(mgos_zvarc_t src_var, mgos_zvar_t dest_var); 
```
Copies the source variable into the destination one. Returns `true` if copied successfully, otherwise `false`.

|Parameter||
|--|--|
|src|Source variant variable.|
|dest|Destination variant variable.|
### mgos_zvar_length()
```c
int mgos_zvar_length(mgos_zvarc_t var); 
```
Returns the number of items in a dictionary the string length. Returns `0` in all other cases.

|Parameter||
|--|--|
|var|Variant variable.|
### mgos_zvar_set_unchanged()
```c
void mgos_zvar_set_unchanged(mgos_zvar_t var);
```
Marks the variable as unchanged. This function could be used in compination with `mgos_zvar_is_changed`.

|Parameter||
|--|--|
|var|Variant variable.|
### mgos_zvar_is_changed()
```c
bool mgos_zvar_is_changed(mgos_zvarc_t var);
```
Returns `true` if the variable value is changed since its creation or since the last call of `mgos_zvar_set_unchanged`, otherwise `false`.

|Parameter||
|--|--|
|var|Variant variable.|
### mgos_zvar_free()
```c
void mgos_zvar_free(mgos_zvar_t var);
```
Deallocates the variable. If the variable is an element of a dictionary, it is also removed from the collection. If the variable is a dictionary, all its items are deallocated as well.

|Parameter||
|--|--|
|var|Variant variable.|
