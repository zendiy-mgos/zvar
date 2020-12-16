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

#include <stdlib.h>
#include <stdbool.h>

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */ 

#ifdef __cplusplus
extern "C" {
#endif

#define MGOS_ZVARP_BIGINT(v) mgos_zvar_bigint_get(v)
#define MGOS_ZVAR_BIGINT(v) mgos_zvar_bigint_get(&v)

#define MGOS_ZVARP_DBL(v) mgos_zvar_decimal_get(v)
#define MGOS_ZVAR_DBL(v) mgos_zvar_decimal_get(&v)

#define MGOS_ZVARP_BOOL(v) mgos_zvar_bool_get(v)
#define MGOS_ZVAR_BOOL(v) mgos_zvar_bool_get(&v)

enum mgos_zvar_type {
  MGOS_ZVARIANT_TYPE_UNK = 0,     //00000
  MGOS_ZVARIANT_TYPE_DIC = 1,   //00001
  MGOS_ZVARIANT_TYPE_BOOL = 2,    //00010
  MGOS_ZVARIANT_TYPE_BIGINT = 4,  //00100
  MGOS_ZVARIANT_TYPE_DECIMAL = 8, //01000
  MGOS_ZVARIANT_TYPE_STR = 16     //10000
};

union mgos_zvar_value {
   long l;
   double d;
   bool b;
   char *s;
};

#define MGOS_ZVARIANT_NAV { MGOS_ZVARIANT_TYPE_UNK, {0}, NULL }

typedef struct mgos_zvariant {
  enum mgos_zvar_type type; 
  union mgos_zvar_value value;
  void *k;
} mgos_zvar_t;

enum mgos_zvar_type mgos_zvar_type(mgos_zvar_t *v);

bool mgos_zvar_equals(mgos_zvar_t *v1, mgos_zvar_t *v2);

mgos_zvar_t *mgos_zvar_copy(mgos_zvar_t *src, mgos_zvar_t *dest);
    
mgos_zvar_t *mgos_zvar_nav_set(mgos_zvar_t *v);
bool mgos_zvar_is_nav(mgos_zvar_t *v);
                                 
mgos_zvar_t *mgos_zvar_bigint_set(mgos_zvar_t *v, long value);
mgos_zvar_t *mgos_zvar_bool_set(mgos_zvar_t *v, bool value);
mgos_zvar_t *mgos_zvar_decimal_set(mgos_zvar_t *v, double value);
mgos_zvar_t *mgos_zvar_str_set(mgos_zvar_t *v, const char *str);

long mgos_zvar_bigint_get(mgos_zvar_t *v);
bool mgos_zvar_bool_get(mgos_zvar_t *v);
double mgos_zvar_decimal_get(mgos_zvar_t *v);
const char *mgos_zvar_str_get(mgos_zvar_t *v);

void mgos_zvar_free(mgos_zvar_t *v);


bool mgos_zvar_is_dic(mgos_zvar_t *v);
void mgos_zvar_dic_clear(mgos_zvar_t *v);
int mgos_zvar_dic_count(mgos_zvar_t *v);
void mgos_zvar_dic_remove(mgos_zvar_t *v, const char *name);
long mgos_zvar_dic_bigint_get(mgos_zvar_t *v, const char *name);
bool mgos_zvar_dic_bool_get(mgos_zvar_t *v, const char *name);
double mgos_zvar_dic_double_get(mgos_zvar_t *v, const char *name);
const char *mgos_zvar_dic_str_get(mgos_zvar_t *v, const char *name);
mgos_zvar_t *mgos_zvar_dic_bigint_set(mgos_zvar_t *v, const char *name, long val);
mgos_zvar_t *mgos_zvar_dic_bool_set(mgos_zvar_t *v, const char *name, bool val);
mgos_zvar_t *mgos_zvar_dic_decimal_set(mgos_zvar_t *v, const char *name, double val);
mgos_zvar_t *mgos_zvar_dic_str_set(mgos_zvar_t *v, const char *name, const char *val);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_ZVAR_H_ */
