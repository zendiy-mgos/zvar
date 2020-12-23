# ZenVar
## Overview
Mongoose OS library implementing ZenVar variant object. A ZenVar is like a javascript `var`. It has not a strong data-type, but it can be any of the following:
* Boolean (`bool` C type)
* Integer (`long` C type)
* Decimal (`double` C type)
* String
* Dictionary (key/value pairs of any type)

**How to initialize a ZenVar** - Initializing a ZenVar is very easy, thanks to the following special MACROs.
```c
// Initialize as NaV (Not a Value)
mgos_zvar_t var = MGOS_ZVAR_NAV;
printf("Is NaV: %s", (mgos_zvar_is_nav(&var) ? "YES" : "NO"));

// Integer
mgos_zvar_t var = MGOS_ZVAR_BIGINT;           // set to default 0(zero)
mgos_zvar_t var = MGOS_ZVAR_BIGINT_SET(22);
printf("Integer value: %ld", mgos_zvar_bigint_get(&var));

// Boolean
mgos_zvar_t var = MGOS_ZVAR_BOOL;             // set to default false
mgos_zvar_t var = MGOS_ZVAR_BOOL_SET(true);
printf("Boolean value: %s", (mgos_zvar_bool_get(&var) == true ? "true" : "false"));

// Decimal
mgos_zvar_t var = MGOS_ZVAR_DECIMAL;          // set to default 0.0
mgos_zvar_t var = MGOS_ZVAR_DECIMAL_SET(9.2);
printf("Decimal value: %f", mgos_zvar_decimal_get(&var));

// String
mgos_zvar_t var = MGOS_ZVAR_STR;              // set to default NULL
mgos_zvar_t var = MGOS_ZVAR_STR_SET("Mike");
printf("String value: '%s'", mgos_zvar_str_get(&var));

// Empty dictionary
mgos_zvar_t var = MGOS_ZVAR_DIC;
printf("Is dictionary: %s", (mgos_zvar_is_dic(&var) ? "YES" : "NO"));
```
**How to allocate a ZenVar** - Allocating a ZenVar is very easy, thanks to the following special MACROs.
```c
// NaV (Not a Value) pointer
MGOS_ZVAR_NAV_NEW(var);
printf("Is NaV: %s", (mgos_zvar_is_nav(var) ? "YES" : "NO"));

// Integer pointer
MGOS_ZVAR_BIGINT_NEW(var, v);
printf("Integer value: %ld", mgos_zvar_bigint_get(var));

// Boolean pointer
MGOS_ZVAR_BOOL_NEW(var, 22);
printf("Boolean value: %s", (mgos_zvar_bool_get(var) == true ? "true" : "false"));

// Decimal pointer
MGOS_ZVAR_DECIMAL_NEW(var, 9.2);
printf("Decimal value: %f", mgos_zvar_decimal_get(var));

// String pointer
MGOS_ZVAR_STR_NEW(var, "Mike");
printf("String value: '%s'", mgos_zvar_str_get(var));

// Empty dictionary pointer
MGOS_ZVAR_DIC_NEW(v);
printf("Is dictionary: %s", (mgos_zvar_is_dic(var) ? "YES" : "NO"));
```
## C/C++ API Reference
### enum mgos_zvar_type
```c
enum mgos_zvar_type {
  MGOS_ZVAR_TYPE_BOOL,
  MGOS_ZVAR_TYPE_BIGINT,
  MGOS_ZVAR_TYPE_DECIMAL,
  MGOS_ZVAR_TYPE_STR
};
```
ZenVar variant data-types.
### mgos_zvar_type()
```c
enum mgos_zvar_type mgos_zvar_type(mgos_zvar_t *var);
```
Returns the variant [data-type](https://github.com/zendiy-mgos/zvar#enum-mgos_zvar_type).

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_equals()
```c
bool mgos_zvar_equals(mgos_zvar_t *var1, mgos_zvar_t *var2);
```
Compares two variants. Returns `true` if they are equal, otherwise `false`. In case of dictionaries, it compares dictionary elements.

|Parameter||
|--|--|
|var1|Variant instance.|
|var2|Variant instance.|
### mgos_zvar_copy()
```c
mgos_zvar_t *mgos_zvar_copy(mgos_zvar_t *src, mgos_zvar_t *dest);
```
Copies the source variant into the destination one. In case of dictionaries, dictionary elements are copied.

|Parameter||
|--|--|
|src|Source variant instance.|
|dest|Destination variant instance.|
### mgos_zvar_nav_set()
```c
mgos_zvar_t *mgos_zvar_nav_set(mgos_zvar_t *var);
```
Sets variant value to NaV (Not a Value).

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_is_nav()
```c
bool mgos_zvar_is_nav(mgos_zvar_t *var);
```
Returns `true` if the variant value is NaV (Not a Value), otherwise `false`.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_bigint_set()
```c
mgos_zvar_t *mgos_zvar_bigint_set(mgos_zvar_t *var, long value);
```
Sets the integer value.

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_bool_set()
```c
mgos_zvar_t *mgos_zvar_bool_set(mgos_zvar_t *var, bool value);
```
Sets the boolean value.

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_decimal_set()
```c
mgos_zvar_t *mgos_zvar_decimal_set(mgos_zvar_t *var, double value);
```
Sets the boolean value.

|Parameter||
|--|--|
|var|Variant instance.|
|value|Value to set.|
### mgos_zvar_str_set()
```c
mgos_zvar_t *mgos_zvar_str_set(mgos_zvar_t *var, const char *str);
```
Sets the string value.

|Parameter||
|--|--|
|var|Variant instance.|
|str|String to set.|
### mgos_zvar_bigint_get()
```c
long mgos_zvar_bigint_get(mgos_zvar_t *var);
```
Gets the integer value.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_bool_get()
```c
bool mgos_zvar_bool_get(mgos_zvar_t *var);
```
Gets the boolean value.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_decimal_get()
```c
double mgos_zvar_decimal_get(mgos_zvar_t *var);
```
Gets the decimal value.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_str_get()
```c
const char *mgos_zvar_str_get(mgos_zvar_t *var);
```
Gets the string value.

|Parameter||
|--|--|
|var|Variant instance.|
### json_printf_zvar()
```c
int json_printf_zvar(struct json_out *out, va_list *ap);
```
Helper %M callback that prints the variant. Consumes a mgos_zvar_t *. Returns number of printed bytes. This helper can be used in `json_printf`, `json_fprintf` or `json_asprintf` functions (see Mongoose OS [JSON library](https://mongoose-os.com/docs/mongoose-os/api/core/frozen.h.md) for more details).
```c
// Example #1: print a decimal variant
mgos_zvar_t var = MGOS_ZVAR_DECIMAL_SET(51.2);
char *buf = json_asprintf("{humidity: %M}", json_printf_zvar, &var);
printf("%s", buf); // '{"humidity": 51.2}'
free(buf):

// Example #2: print a dictionary
mgos_zvar_t var = MGOS_ZVAR_DIC;
mgos_zvar_dic_decimal_set(&var, "humidity", 51.2);
mgos_zvar_dic_bool_set(&var, "rain", true);
mgos_zvar_dic_str_set(&var, "weather", "rainy");
char *buf = json_asprintf("{state: %M}", json_printf_zvar, &var);
printf("%s", buf); // '{"state": {"humidity": 51.2, "rain": true, "weather": "rainy"}}'
free(buf):
```
### mgos_zvar_free()
```c
void mgos_zvar_free(mgos_zvar_t *var);
```
Closes and disposes variant instance.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_is_dic()
```c
bool mgos_zvar_is_dic(mgos_zvar_t *var);
```
Returns `true` if the variant is a dictionary, otherwise `false`.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_clear()
```c
void mgos_zvar_dic_clear(mgos_zvar_t *var);
```
Removes all dictionary elements.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_count()
```c
int mgos_zvar_dic_count(mgos_zvar_t *var);
```
Returns the number of elements in the dictionary.

|Parameter||
|--|--|
|var|Variant instance.|
### mgos_zvar_dic_remove()
```c
void mgos_zvar_dic_remove(mgos_zvar_t *var, const char *key);
```
Removes the element from the dictionary.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_contains()
```c
bool mgos_zvar_dic_contains(mgos_zvar_t *var, const char *key);
```
Returns `true` if the dictionary contains the element key, otherwise `false`. 

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|

### mgos_zvar_dic_get()
```c
mgos_zvar_t *mgos_zvar_dic_get(mgos_zvar_t *var, const char *key);
```
Returns the variant element. 

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_bigint_get()
```c
long mgos_zvar_dic_bigint_get(mgos_zvar_t *var, const char *key);
```
Gets the integer value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_bool_get()
```c
bool mgos_zvar_dic_bool_get(mgos_zvar_t *var, const char *key);
```
Gets the boolean value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_decimal_get()
```c
double mgos_zvar_dic_decimal_get(mgos_zvar_t *var, const char *key);
```
Gets the decimal value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_str_get()
```c
const char *mgos_zvar_dic_str_get(mgos_zvar_t *var, const char *key);
```
Gets the string value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
### mgos_zvar_dic_bigint_set()
```c
mgos_zvar_t *mgos_zvar_dic_bigint_set(mgos_zvar_t *var, const char *key, long val);
```
Sets the integer value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
|val|Value to set.|
### mgos_zvar_dic_bool_set()
```c
mgos_zvar_t *mgos_zvar_dic_bool_set(mgos_zvar_t *var, const char *key, bool val);
```
Sets the boolean value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
|val|Value to set.|
### mgos_zvar_t *mgos_zvar_dic_decimal_set()
```c
mgos_zvar_t *mgos_zvar_dic_decimal_set(mgos_zvar_t *var, const char *key, double val);
```
Sets the decimal value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
|val|Value to set.|
### mgos_zvar_dic_str_set()
```c
mgos_zvar_t *mgos_zvar_dic_str_set(mgos_zvar_t *var, const char *key, const char *str);
```
Sets the string value of the dictionary element.

|Parameter||
|--|--|
|var|Variant instance.|
|key|Key name.|
|str|String to set.|
### mgos_zvar_dic_get_at()
```c
mgos_zvar_t *mgos_zvar_dic_get_at(mgos_zvar_t *var, int idx, const char **key);
```
Gets the variant instance of the dictionary element at specified zero-based index. If the `key` parameter is not `NULL`, the element key name is returned. 

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index (zero-based).|
|key|Key name of returned element (optional).|
### mgos_zvar_dic_bigint_get_at()
```c
long mgos_zvar_dic_bigint_get_at(mgos_zvar_t *var, int idx, const char **key);
```
Gets the integer value of the dictionary element at specified zero-based index. If the `key` parameter is not `NULL`, the element key name is returned.

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index (zero-based).|
|key|Key name of returned element (optional).|
### mgos_zvar_dic_bool_get_at()
```c
bool mgos_zvar_dic_bool_get_at(mgos_zvar_t *var, int idx, const char **key);
```
Gets the boolean value of the dictionary element at specified zero-based index. If the `key` parameter is not `NULL`, the element key name is returned.

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index (zero-based).|
|key|Key name of returned element (optional).|
### mgos_zvar_dic_decimal_get_at()
```c
double mgos_zvar_dic_decimal_get_at(mgos_zvar_t *var, int idx, const char **key);
```
Gets the decimal value of the dictionary element at specified zero-based index. If the `key` parameter is not `NULL`, the element key name is returned.

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index (zero-based).|
|key|Key name of returned element (optional).|
### mgos_zvar_dic_str_get_at()
```c
const char *mgos_zvar_dic_str_get_at(mgos_zvar_t *var, int idx, const char **key);
```
Gets the string value of the dictionary element at specified zero-based index. If the `key` parameter is not `NULL`, the element key name is returned.

|Parameter||
|--|--|
|var|Variant instance.|
|idx|Item index (zero-based).|
|key|Key name of returned element (optional).|
## JS API Reference
### Data-types
```js
ZenVar.TYPE.BOOL
ZenVar.TYPE.BIGINT
ZenVar.TYPE.DECIMAL
ZenVar.TYPE.STR
```
### .typeOf()
```js
let type = ZenVar.typeOf(v);
```
Returns the variant [data-type](https://github.com/zendiy-mgos/zvar#data-types).

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
### .isNaV(c)
```js
let isNav = ZenVar.isNaV(v);
```
Returns `true` if the variant value is NaV (Not a Value), otherwise `false`.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
### .NaV()
```js
ZenVar.NaV(v);
```
Sets variant value to NaV (Not a Value).

|Parameter|Type||
|--|--|--|
|src|object|Variant instance pointer (mgos_zvar_ *)|
### .isDictionary()
```js
let isDic = ZenVar.isDictionary(v);
```
Returns `true` if the variant is a dictionary, otherwise `false`.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
### .equals()
```js
let eq = ZenVar.equals(v1, v2);
```
Compares two variants. Returns `true` if they are equal, otherwise `false`. In case of dictionaries, it compares dictionary elements.

|Parameter|Type||
|--|--|--|
|v1|object|Variant instance pointer (mgos_zvar_ *)|
|v2|object|Variant instance pointer (mgos_zvar_ *)|
### .copy()
```js
ZenVar.copy(src, dest);
```
Copies the source variant into the destination one. In case of dictionaries, dictionary elements are copied.

|Parameter|Type||
|--|--|--|
|src|object|Source variant instance pointer (mgos_zvar_ *)|
|dest|object|Destination variant instance pointer (mgos_zvar_ *)|
### .bool()
```js
// get the value
let out = ZenVar.bool(v);
// set the value
ZenVar.bool(v, val);
```
Gest or sets the boolean value.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *).|
|val|boolean|Value to set.|
### .int()
```js
// get the value
let out = ZenVar.int(v);
// set the value
ZenVar.int(v, val);
```
Gest or sets the integer value.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *).|
|val|integer|Value to set.|
### .decimal()
```js
// get the value
let out = ZenVar.decimal(v);
// set the value
ZenVar.decimal(v, val);
```
Gest or sets the decimal value.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *).|
|val|decimal|Value to set.|
### .string()
```js
// get the value
let out = ZenVar.string(v);
// set the value
ZenVar.string(v, val);
```
Gest or sets the string value.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *).|
|val|string|Value to set.|
### .dictionary().count()
```js
let count = ZenVar.dictionary(v).count();
```
Returns the number of elements in the dictionary.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
### .dictionary().clear()
```js
ZenVar.dictionary(v).clear();
```
Removes all dictionary elements.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
### .dictionary().remove()
```js
ZenVar.dictionary(v, key).remove();
```
Removes the element from the dictionary. 

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
### .dictionary().exists()
```js
let exists = ZenVar.dictionary(v, key).exists();
```
Returns `true` if the dictionary contains the element key, otherwise `false`.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
### .dictionary().bool()
```js
// get the value
let out = ZenVar.dictionary(v, key).bool();
// set the value
ZenVar.dictionary(v, key).bool(val)
```
Sets or gets the boolean value of the dictionary element.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
|val|boolean|Value to set.|
### .dictionary().int()
```js
// get the value
let out = ZenVar.dictionary(v, key).int();
// set the value
ZenVar.dictionary(v, key).int(val)
```
Sets or gets the integer value of the dictionary element.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
|val|integer|Value to set.|
### .dictionary().decimal()
```js
// get the value
let out = ZenVar.dictionary(v, key).decimal();
// set the value
ZenVar.dictionary(v, key).decimal(val)
```
Sets or gets the decimal value of the dictionary element.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
|val|decimal|Value to set.|
### .dictionary().string()
```js
// get the value
let out = ZenVar.dictionary(v, key).string();
// set the value
ZenVar.dictionary(v, key).string(val)
```
Sets or gets the string value of the dictionary element.

|Parameter|Type||
|--|--|--|
|v|object|Variant instance pointer (mgos_zvar_ *)|
|key|string|Element key name.|
|val|string|Value to set.|