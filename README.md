# ZenVar
## Overview
This Mongoose OS library allows you to create variant variables which haven't data type declared explicitly (like using `var` statement in javascript). You can create variables that can easily and dynamically assume any data-type within the followings:

- Boolean (`bool`)
- Integer (`long`)
- Decimal (`double`)
- String (`char *`)
- Dictionary (key/value pair dictionary) - This requires you to include the [ZenVar Dictionary library](https://github.com/zendiy-mgos/zvar-dic) in your porject.
## Features
- **Observable value** - You can check it the value of a variable is changed or not.
- **JSON support** - You can dynamically create a variant varibale from a JSON string or you can save it as JSON in a very easy way. Just include the [ZenVar JSON library](https://github.com/zendiy-mgos/zvar-json) into your project. 
## Get Started
Include the library into your `mos.yml` file.
```yaml
libs:
  - origin: https://github.com/zendiy-mgos/zvar
```
**C/C++ sample code**

Create new variant variables.
```c
#include "mgos_zvar.h"

mgos_zvar_t n = mgos_zvar_new();                  // void *n = NULL; (type-less var)
mgos_zvar_t i = mgos_zvar_new_integer(101);       // long i = 101;
mgos_zvar_t b = mgos_zvar_new_bool(true);         // bool b = true;
mgos_zvar_t d = mgos_zvar_new_decimal(101.99);    // double d = 101.99;
mgos_zvar_t s = mgos_zvar_new_str("Lorem Ipsum"); // char *s = "Lorem Ipsum";
```
## C/C++ APIs Reference
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
Returns the variable [data-type](https://github.com/zendiy-mgos/zvar#enum-mgos_zvar_get_type). Returns `MGOS_ZVAR_TYPE_DIC` if the variable is a dictionary.

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_new
```c
mgos_zvar_t mgos_zvar_new();
```
Creates a type-less variable (with no data-type defined). Returns `NULL` if error. The returned instance must be deallocated using `mgos_zvar_free`.
### mgos_zvar_new_integer, mgos_zvar_new_bool, mgos_zvar_new_decimal and mgos_zvar_new_str
```c       
mgos_zvar_t mgos_zvar_new_integer(long value);
mgos_zvar_t mgos_zvar_new_bool(bool value);
mgos_zvar_t mgos_zvar_new_decimal(double value);
mgos_zvar_t mgos_zvar_new_str(const char *value);
```
Creates and initializes a variable. Returns `NULL` if error. Invoking `mgos_zvar_new_str(NULL)` is equivalent to `mgos_zvar_new()`. The returned instance must be deallocated using `mgos_zvar_free`.

|Parameter||
|--|--|
|value|Value to be set.|
### mgos_zvar_set_null
```c 
void mgos_zvar_set_null(mgos_zvar_t var);
```
Sets a variable as type-less (with no data-type defined).

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_set_integer, mgos_zvar_set_bool, mgos_zvar_set_decimal and mgos_zvar_set_str
```c                                 
void mgos_zvar_set_integer(mgos_zvar_t var, long value);
void mgos_zvar_set_bool(mgos_zvar_t var, bool value);
void mgos_zvar_set_decimal(mgos_zvar_t var, double value);
void mgos_zvar_set_str(mgos_zvar_t var, const char *value);
```
Sets a variable value and data-type. Invoking `mgos_zvar_set_str(var, NULL)` is equivalent to `mgos_zvar_set_null(var)`.

|Parameter||
|--|--|
|var|A variant variable.|
|value|Value to set.|
### mgos_zvar_set_nstr
```c 
void mgos_zvar_set_nstr(mgos_zvar_t var, const char *value, size_t value_len);
```
Sets a variable value to the provided string. This is a specialized version of `mgos_zvar_set_str` Invoking `mgos_zvar_set_nstr(var, NULL, <any_value>)` is equivalent to `mgos_zvar_set_null(var)`.

|Parameter||
|--|--|
|var|A variant variable.|
|value|String value to set.|
|value_len|Maximum number of characters to set. Ignored if `value` parameter is `NULL`.|
### mgos_zvar_get_integer, mgos_zvar_get_bool, mgos_zvar_get_decimal and mgos_zvar_get_str
```c 
long mgos_zvar_get_integer(mgos_zvarc_t var);
bool mgos_zvar_get_bool(mgos_zvarc_t var);
double mgos_zvar_get_decimal(mgos_zvarc_t var);
const char *mgos_zvar_get_str(mgos_zvarc_t var);
```
Returns the value of a variable.

|Parameter||
|--|--|
|var|A variant variable.|

**Remarks**

The returned value depends on the input variable data-type. Please refer to details below.
|Function / Input|INTEGER|BOOL|DECIMAL|STRING|Any other|
|--|--|--|--|--|--|
|mgos_zvar_get_integer|Returns the integer value|Returns `0` if input value is `false`|Returns the integer part of the decimal|Returns `0`|Returns `0`|
|mgos_zvar_get_bool|Returns `false` if input value is `0`|Returns the boolean value|Returns `false` if input value is `0.0`|Returns `false` if input string is empty|Returns `false`|
|mgos_zvar_get_decimal|Returns the input value as decimal|Returns `0.0`|Returns the decimal value|Returns `0.0`|Returns `0.0`|
|mgos_zvar_get_str|Returns `NULL`|Returns `NULL`|Returns `NULL`|Returns the string value|Returns `NULL`|
### mgos_zvar_cmp
```c
int mgos_zvar_cmp(mgos_zvarc_t var1, mgos_zvarc_t var2);
```
Compares variable *var1* to variable *var2*. Returns `INT_MAX` if error.

|Parameter||
|--|--|
|var1|A variant variable.|
|var2|A variant variable.| 

**Remarks**

Returns an integer value indicating the relationship between the variables:
|Return value||
|--|--|
|<0|The value of *var1* is minor than the value of *var2*. If one or both of the variables are dictionaries, they are not equal.|
|0|The two variables are equal. If both of the variables are dictionaries, they contain the same keys, regardless their order.|
|>0|The value of *var1* is minor than the value of *var2*.|
### mgos_zvar_is_null
```c
bool mgos_zvar_is_null(mgos_zvarc_t var);
```
Returns `true` if the variable value is type-less (with no data-type defined), or `false` otherwise.

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_copy
```c
bool mgos_zvar_copy(mgos_zvarc_t src_var, mgos_zvar_t dest_var); 
```
Copies a source variable into the destination one. Returns `true` if successfully copied, or `false` otherwise.

|Parameter||
|--|--|
|src_var|Source variant variable.|
|dest_var|Destination variant variable.|
### mgos_zvar_length
```c
int mgos_zvar_length(mgos_zvarc_t var); 
```
Returns the number of items in a dictionary or the string length. Returns `0` in all other cases.

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_set_unchanged
```c
void mgos_zvar_set_unchanged(mgos_zvar_t var);
```
Marks the variable as unchanged. This function could be used in combination with `mgos_zvar_is_changed`.

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_is_changed
```c
bool mgos_zvar_is_changed(mgos_zvarc_t var);
```
Returns `true` if the variable value is changed since its creation or since the last call of `mgos_zvar_set_unchanged`, or `false` otherwise.

|Parameter||
|--|--|
|var|A variant variable.|
### mgos_zvar_free
```c
void mgos_zvar_free(mgos_zvar_t var);
```
Deallocates the variable. If the variable is an element of a dictionary, it is also removed from the collection. If the variable is a dictionary, all its items are deallocated as well.

|Parameter||
|--|--|
|var|A variant variable.|
## To Do
- Implement variant array.
- Implement javascript APIs for [Mongoose OS MJS](https://github.com/mongoose-os-libs/mjs).
