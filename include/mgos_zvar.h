/*
 * Copyright (c) 2020 ZenDIY
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MGOS_ZVAR_H_
#define MGOS_ZVAR_H_

/* Uncomment lines below for running unit tests
 * (run unit-tests.c)
 */
//#define MGOS_ZVAR_HAVE_DIC 1
//#define MGOS_ZVAR_HAVE_JSON 1

#include <stdlib.h>
#include <stdbool.h>

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */ 

#ifdef __cplusplus
extern "C" {
#endif

enum mgos_zvar_type {
  MGOS_ZVAR_TYPE_NULL = 0,
  MGOS_ZVAR_TYPE_BOOL = 2,
  MGOS_ZVAR_TYPE_INTEGER = 3,
  MGOS_ZVAR_TYPE_DECIMAL = 4,
  MGOS_ZVAR_TYPE_STR = 5
};

struct mgos_zvariant;
typedef struct mgos_zvariant *mgos_zvar_t;
typedef const struct mgos_zvariant *mgos_zvarc_t;

mgos_zvar_t mgos_zvar_new();            
mgos_zvar_t mgos_zvar_new_integer(long value);
mgos_zvar_t mgos_zvar_new_bool(bool value);
mgos_zvar_t mgos_zvar_new_decimal(double value);
mgos_zvar_t mgos_zvar_new_str(const char *value);

void mgos_zvar_set_null(mgos_zvar_t var);                                 
void mgos_zvar_set_integer(mgos_zvar_t var, long value);
void mgos_zvar_set_bool(mgos_zvar_t var, bool value);
void mgos_zvar_set_decimal(mgos_zvar_t var, double value);
void mgos_zvar_set_str(mgos_zvar_t var, const char *value);
void mgos_zvar_set_nstr(mgos_zvar_t var, const char *value, size_t value_len);

long mgos_zvar_get_integer(mgos_zvarc_t var);
bool mgos_zvar_get_bool(mgos_zvarc_t var);
double mgos_zvar_get_decimal(mgos_zvarc_t var);
const char *mgos_zvar_get_str(mgos_zvarc_t var);

enum mgos_zvar_type mgos_zvar_get_type(mgos_zvarc_t var);
bool mgos_zvar_copy(mgos_zvarc_t src_var, mgos_zvar_t dest_var);  
bool mgos_zvar_is_null(mgos_zvarc_t var);

bool mgos_zvar_is_equal(mgos_zvarc_t var1, mgos_zvarc_t var2);

int mgos_zvar_length(mgos_zvarc_t var);

void mgos_zvar_set_unchanged(mgos_zvar_t var);
bool mgos_zvar_is_changed(mgos_zvarc_t var);

void mgos_zvar_free(mgos_zvar_t var);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_ZVAR_H_ */