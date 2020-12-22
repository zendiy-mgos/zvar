#include <string.h>
#include <stdarg.h>
#include "mgos_zvar.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

extern bool mgos_zvar_is_dic(mgos_zvar_t *);
extern void mgos_zvar_dic_clear(mgos_zvar_t *);
extern bool mg_zvar_dic_equals(mgos_zvar_t *, mgos_zvar_t *);
extern mgos_zvar_t *mg_zvar_dic_copy(mgos_zvar_t *, mgos_zvar_t *);
extern int mg_zvar_dic_json_printf(struct json_out *, mgos_zvar_t *v);

enum mgos_zvar_type mgos_zvar_type(mgos_zvar_t *v) {
  if (v){
    return (mgos_zvar_is_dic(v) ? (v->type ^ MGOS_ZVAR_TYPE_DIC) : v->type);
  }
  return MGOS_ZVAR_TYPE_UNK;
}

void mg_zvar_type_set(mgos_zvar_t *v, enum mgos_zvar_type t) {
  if(v) {
    v->type = (mgos_zvar_is_dic(v) ? (MGOS_ZVAR_TYPE_DIC | t) : t);
  }
}

bool mgos_zvar_is_nav(mgos_zvar_t *v) {
  return (mgos_zvar_type(v) == MGOS_ZVAR_TYPE_UNK);
}

void mg_zvar_close(mgos_zvar_t *v) {
  if (v) {
    if ((v->type & MGOS_ZVAR_TYPE_STR) == MGOS_ZVAR_TYPE_STR) {
      free(v->value.s);
      v->value.s = NULL;
    }
    v->v_size = 0;
  }
}

mgos_zvar_t *mg_zvar_nav_set(mgos_zvar_t *v, bool skip_dic) {
  if (v) {
    if (!skip_dic && mgos_zvar_is_dic(v)) {
      mgos_zvar_dic_clear(v);
    } else {
      mg_zvar_close(v);
    }
    mg_zvar_type_set(v, MGOS_ZVAR_TYPE_UNK);
  }
  return v;
}

mgos_zvar_t *mgos_zvar_nav_set(mgos_zvar_t *v) {
  mg_zvar_nav_set(v, false)->type = MGOS_ZVAR_TYPE_UNK;
  return v;
}

bool mg_zvar_equals(mgos_zvar_t *v1, mgos_zvar_t *v2, bool skip_dic) {
  if (v1 != NULL && v2 != NULL) {
    
    if (!skip_dic) {
      if(mgos_zvar_is_dic(v1) || mgos_zvar_is_dic(v2)) {
        return mg_zvar_dic_equals(v1, v2);
      }
    }

    if (v1->type != v2->type) return false;
    switch(mgos_zvar_type(v1)) {
      case MGOS_ZVAR_TYPE_BIGINT:
        return (v1->value.l == v2->value.l);
      case MGOS_ZVAR_TYPE_DECIMAL:
        return (v1->value.d == v2->value.d);
      case MGOS_ZVAR_TYPE_BOOL:
        return (v1->value.b == v2->value.b);
      case MGOS_ZVAR_TYPE_STR:
        return (strcmp(v1->value.s, v2->value.s) == 0);
      case MGOS_ZVAR_TYPE_UNK:
        return true;
      default:
        return false;
    };
  }
  return (v1 == NULL && v2 == NULL);
}

bool mgos_zvar_equals(mgos_zvar_t *v1, mgos_zvar_t *v2) {
  return  mg_zvar_equals(v1, v2, false);
}

mgos_zvar_t *mg_zvar_copy(mgos_zvar_t *src, mgos_zvar_t *dest, bool skip_dic) {
  if (src) {
    if (!skip_dic && mgos_zvar_is_dic(src)) {
      return mg_zvar_dic_copy(src, dest);
    }

    switch(mgos_zvar_type(src)) {
      case MGOS_ZVAR_TYPE_BIGINT:
        return mgos_zvar_bigint_set(dest, src->value.l);
      case MGOS_ZVAR_TYPE_DECIMAL:
        return mgos_zvar_decimal_set(dest, src->value.d);
      case MGOS_ZVAR_TYPE_BOOL:
        return mgos_zvar_bool_set(dest, src->value.b);
      case MGOS_ZVAR_TYPE_STR:
        return mgos_zvar_str_set(dest, (const char *)src->value.s);
      case MGOS_ZVAR_TYPE_UNK:
        return mg_zvar_nav_set(dest, skip_dic);
      default:
        return NULL;
    };
  }
  return dest;
}

mgos_zvar_t *mgos_zvar_copy(mgos_zvar_t *src, mgos_zvar_t *dest) {
  return mg_zvar_copy(src, dest, false);
}

mgos_zvar_t *mgos_zvar_bigint_set(mgos_zvar_t *v, long value) {
  if (v) {
    mg_zvar_close(v);
    mg_zvar_type_set(v, MGOS_ZVAR_TYPE_BIGINT);
    v->value.l = value;
    v->v_size = sizeof(long);
  }
  return v;
}

mgos_zvar_t *mgos_zvar_bool_set(mgos_zvar_t *v, bool value) {
  if (v) {
    mg_zvar_close(v);
    mg_zvar_type_set(v, MGOS_ZVAR_TYPE_BOOL);
    v->value.b = value;
    v->v_size = sizeof(bool);
  }
  return v;
}

mgos_zvar_t *mgos_zvar_decimal_set(mgos_zvar_t *v, double value) {
  if (v) {
    mg_zvar_close(v);
    mg_zvar_type_set(v, MGOS_ZVAR_TYPE_DECIMAL);
    v->value.d = value;
    v->v_size = sizeof(double);
  }
  return v;
}

mgos_zvar_t *mgos_zvar_str_set(mgos_zvar_t *v, const char *str) {
  if (v) {
    // try to optimize string allocation
    if ((mgos_zvar_type(v) == MGOS_ZVAR_TYPE_STR) && str && v->value.s) {
      if (strlen(str) < v->v_size) {
        // re-use previously allocated buffer
        strcpy(v->value.s, str);
        return v;
      }
    }

    mg_zvar_nav_set(v, true);
    if (str) {
      mg_zvar_type_set(v, MGOS_ZVAR_TYPE_STR);
      v->value.s = strdup(str);
      v->v_size = (strlen(str) + 1);
    }
  }
  return v;
}

long mgos_zvar_bigint_get(mgos_zvar_t *v) {
  return ((mgos_zvar_type(v) == MGOS_ZVAR_TYPE_BIGINT) ? v->value.l : 0);
}

bool mgos_zvar_bool_get(mgos_zvar_t *v) {
  return ((mgos_zvar_type(v) == MGOS_ZVAR_TYPE_BIGINT) ? v->value.b : false);
}

double mgos_zvar_decimal_get(mgos_zvar_t *v) {
  return ((mgos_zvar_type(v) == MGOS_ZVAR_TYPE_DECIMAL) ? v->value.d : 0);
}

const char *mgos_zvar_str_get(mgos_zvar_t *v) {
  return ((mgos_zvar_type(v) == MGOS_ZVAR_TYPE_STR) ? v->value.s : NULL);
}

void mg_zvar_free(mgos_zvar_t *v, bool skip_dic) {
  if (v) {
    if (!skip_dic && ((v->type & MGOS_ZVAR_TYPE_DIC) == MGOS_ZVAR_TYPE_DIC)) {
      mgos_zvar_dic_clear(v);
    } else {
      mg_zvar_close(v);    
    }
    free(v);
  }
}

int mg_zvar_json_printf(struct json_out *out, mgos_zvar_t *v) {
  const char *fmt;
  switch(mgos_zvar_type(v)) {
    case MGOS_ZVAR_TYPE_BIGINT:
      fmt = "%ld";
      break;
    case MGOS_ZVAR_TYPE_DECIMAL:
      fmt = "%f";
      break;
    case MGOS_ZVAR_TYPE_BOOL:
      fmt = "%B";
      break;
    case MGOS_ZVAR_TYPE_STR:
      fmt = "%Q";
      break;
    default:
      return json_printf(out, "%Q", NULL);
  };
  return json_printf(out, fmt, v->value);
}

int json_printf_zvar(struct json_out *out, va_list *ap) {
  mgos_zvar_t *v = va_arg(*ap, void *);
  if (mgos_zvar_is_dic(v)) {
    return mg_zvar_dic_json_printf(out, v);
  }
  return mg_zvar_json_printf(out, v);
}

void mgos_zvar_free(mgos_zvar_t *v) {
  mg_zvar_free(v, false);
}

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */

bool mgos_zvar_init() {
  return true;
}