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
#include <string.h>
#include "frozen.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */ 

#ifdef __cplusplus
extern "C" {
#endif

enum mgos_zvar_type {
  MGOS_ZVAR_TYPE_UNK = 0,     //00000
  MGOS_ZVAR_TYPE_DIC = 1,     //00001
  MGOS_ZVAR_TYPE_BOOL = 2,    //00010
  MGOS_ZVAR_TYPE_BIGINT = 4,  //00100
  MGOS_ZVAR_TYPE_DECIMAL = 8, //01000
  MGOS_ZVAR_TYPE_STR = 16     //10000
};

union mgos_zvar_value {
   long l;
   double d;
   bool b;
   char *s;
};

#define MG_ZVAR_INIT(t,v,uf) { .type=t, .value.uf = v, .k = NULL }

#define MGOS_ZVAR_NAV               MG_ZVAR_INIT(MGOS_ZVAR_TYPE_UNK, 0, d)
#define MGOS_ZVAR_DIC               MG_ZVAR_INIT(MGOS_ZVAR_TYPE_DIC, 0, d)

#define MGOS_ZVAR_BIGINT_SET(v)     MG_ZVAR_INIT(MGOS_ZVAR_TYPE_BIGINT, v, l)
#define MGOS_ZVAR_BOOL_SET(v)       MG_ZVAR_INIT(MGOS_ZVAR_TYPE_BOOL, v, b)
#define MGOS_ZVAR_DECIMAL_SET(v)    MG_ZVAR_INIT(MGOS_ZVAR_TYPE_DECIMAL, v, d)
#define MGOS_ZVAR_STR_SET(v)        MG_ZVAR_INIT(MGOS_ZVAR_TYPE_STR, (v ? strdup(v) : 0), s)

#define MGOS_ZVAR_BIGINT            MGOS_ZVAR_BIGINT_SET(0)
#define MGOS_ZVAR_BOOL              MGOS_ZVAR_BOOL_SET(false)
#define MGOS_ZVAR_DECIMAL           MGOS_ZVAR_DECIMAL_SET(0.0)
#define MGOS_ZVAR_STR               MGOS_ZVAR_STR_SET(NULL)

#define MG_ZVAR_NEW(n, t, v, uf)    mgos_zvar_t *n = calloc(1, sizeof(void *)); n->type = t; n->value.uf = v;

#define MGOS_ZVAR_NAV_NEW(n)        MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_UNK, 0, d)
#define MGOS_ZVAR_DIC_NEW(n)        MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_DIC, 0, d)
#define MGOS_ZVAR_BIGINT_NEW(n, v)   MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_BIGINT, v, l)
#define MGOS_ZVAR_BOOL_NEW(n, v)     MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_BOOL, v, b)
#define MGOS_ZVAR_DECIMAL_NEW(n, v)  MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_DECIMAL, v, d)
#define MGOS_ZVAR_STR_NEW(n, v)      MG_ZVAR_NEW(n, MGOS_ZVAR_TYPE_STR, strdup(v), s)


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

int json_printf_zvar(struct json_out *out, va_list *ap);

void mgos_zvar_free(mgos_zvar_t *v);


/* Dictionary functions */

bool mgos_zvar_is_dic(mgos_zvar_t *v);
void mgos_zvar_dic_clear(mgos_zvar_t *v);
int mgos_zvar_dic_count(mgos_zvar_t *v);
void mgos_zvar_dic_remove(mgos_zvar_t *v, const char *name);

long mgos_zvar_dic_bigint_get(mgos_zvar_t *v, const char *name);
bool mgos_zvar_dic_bool_get(mgos_zvar_t *v, const char *name);
double mgos_zvar_dic_decimal_get(mgos_zvar_t *v, const char *name);
const char *mgos_zvar_dic_str_get(mgos_zvar_t *v, const char *name);

mgos_zvar_t *mgos_zvar_dic_bigint_set(mgos_zvar_t *v, const char *name, long val);
mgos_zvar_t *mgos_zvar_dic_bool_set(mgos_zvar_t *v, const char *name, bool val);
mgos_zvar_t *mgos_zvar_dic_decimal_set(mgos_zvar_t *v, const char *name, double val);
mgos_zvar_t *mgos_zvar_dic_str_set(mgos_zvar_t *v, const char *name, const char *val);

mgos_zvar_t *mgos_zvar_dic_get_at(mgos_zvar_t *v, int idx, const char **key);
long mgos_zvar_dic_bigint_get_at(mgos_zvar_t *v, int idx, const char **key);
bool mgos_zvar_dic_bool_get_at(mgos_zvar_t *v, int idx, const char **key);
double mgos_zvar_dic_decimal_get_at(mgos_zvar_t *v, int idx, const char **key);
const char *mgos_zvar_dic_str_get_at(mgos_zvar_t *v, int idx, const char **key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_ZVAR_H_ */
