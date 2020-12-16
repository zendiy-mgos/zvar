#include <string.h>
#include "mgos_zvar.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

extern bool mg_zvar_equals(mgos_zvar_t *, mgos_zvar_t *, bool);
extern void mg_zvar_free(mgos_zvar_t *, bool);
extern mgos_zvar_t *mg_zvar_copy(mgos_zvar_t *, mgos_zvar_t *, bool);
extern mgos_zvar_t *mg_zvar_nav_set(mgos_zvar_t *, bool);
extern void mg_zvar_type_set(mgos_zvar_t *, enum mgos_zvar_type);

struct mg_zvar_dic_k {
  const char* name;
  mgos_zvar_t *value;
  struct mg_zvar_dic_k *n;
  struct mg_zvar_dic_k *p;
};

int mgos_zvar_dic_count(mgos_zvar_t *v) {
  int count = 0;
  struct mg_zvar_dic_k *k = (v ? v->k : NULL);
  while(k) { ++count; k = k->n; };
  return count;
}

mgos_zvar_t *mg_zvar_dic_get(mgos_zvar_t *root, const char *name, bool create) {
  if (!root || !name) return NULL;
  mgos_zvar_t *i = root;
  while (i && i->k) {
    if (strcmp(((struct mg_zvar_dic_k *)i->k)->name, name) == 0) return i;
    i = (((struct mg_zvar_dic_k *)i->k)->n ? ((struct mg_zvar_dic_k *)i->k)->n->value : NULL);
  }
  // not found...
  if (create) {
    // create a new variant if needed
    if (!i) i = calloc(1, sizeof(mgos_zvar_t));
    i->type |= MGOS_ZVARIANT_TYPE_DIC;
    struct mg_zvar_dic_k *last_k = root->k;
    // find last key
    while(last_k && last_k->n) { last_k = last_k->n; };
    // create a new key
    struct mg_zvar_dic_k *k = calloc(1, sizeof(struct mg_zvar_dic_k));
    k->name = strdup(name);
    // attach variant and key
    i->k = k;
    k->value = i;
    // attach key to the last key
    if (last_k) {
      last_k->n = k;
      k->p = last_k;
    }
  }
  return i;
}

bool mg_zvar_dic_equals(mgos_zvar_t *v1, mgos_zvar_t *v2) {
  if (!mgos_zvar_is_dic(v1) || !mgos_zvar_is_dic(v2)) return false;
  if (mgos_zvar_dic_count(v1) != mgos_zvar_dic_count(v2)) return false;
  struct mg_zvar_dic_k *k = v1->k;
  while(k) {
    mgos_zvar_t *f = mg_zvar_dic_get(v2, k->name, false);
    if (!f) return false; 
    if (!mg_zvar_equals(f, k->value, true)) return false;
    k = k->n;
  }
  return true;
}

void mg_zvar_dic_remove(struct mg_zvar_dic_k *k) {
  if (k) {
    if (!k->p) {
      if (k->n) {
        mg_zvar_copy(k->n->value, k->value, true);
        free((char *)k->name);
        k->name = strdup(k->n->name);
        mg_zvar_dic_remove(k->n);
        return;
      } else {
        mg_zvar_nav_set(k->value, true);
        k->value->k = NULL;
      }
    } else {
      mg_zvar_free(k->value, true);
      k->p->n = k->n;
    }
    if (k->n) k->n->p = k->p;
    free((char *)k->name);
    free(k);
  }
}

mgos_zvar_t *mg_zvar_dic_copy(mgos_zvar_t *src, mgos_zvar_t *dest) {
  // Sync existing keys and add new keys
  struct mg_zvar_dic_k *k = src->k;
  while(k) {
    mgos_zvar_t *f = mg_zvar_dic_get(dest, k->name, true);
    if (!mg_zvar_equals(f, k->value, true)) {
      mg_zvar_copy(k->value, f, true);
    }
    k = k->n;
  }

  // Remove not existing keys 
  k = dest->k;
  while(k) {
    if (!mg_zvar_dic_get(src, k->name, false)) {
      struct mg_zvar_dic_k *rk = k;
      k = k->n;
      mg_zvar_dic_remove(rk);
    } else {
      k = k->n;
    }
  }
  
  return dest;
}

bool mgos_zvar_is_dic(mgos_zvar_t *v) {
  return (v ? ((v->type & MGOS_ZVARIANT_TYPE_DIC) == MGOS_ZVARIANT_TYPE_DIC) : false);
}

void mgos_zvar_dic_clear(mgos_zvar_t *v) {
  struct mg_zvar_dic_k *tk;
  struct mg_zvar_dic_k *k = (v ? v->k : NULL);
  while(k) {
    if (k->value != v) {
      mg_zvar_free(k->value, true);
    } else {
      mg_zvar_type_set(k->value, MGOS_ZVARIANT_TYPE_UNK);
      k->value->k = NULL;
    }
    
    tk = k->n;
    free((char *)k->name);
    free(k);
    k = tk;
  }
}

void mgos_zvar_dic_remove(mgos_zvar_t *v, const char *name) {
  mgos_zvar_t *i = mg_zvar_dic_get(v, name, false);
  if (i) {
    mg_zvar_dic_remove((struct mg_zvar_dic_k *)i->k);
  }
}

mgos_zvar_t *mgos_zvar_dic_bigint_set(mgos_zvar_t *v, const char *name, long val) {
  mgos_zvar_bigint_set(mg_zvar_dic_get(v, name, true), val);
  return v;
}

mgos_zvar_t *mgos_zvar_dic_decimal_set(mgos_zvar_t *v, const char *name, double val) {
  mgos_zvar_decimal_set(mg_zvar_dic_get(v, name, true), val);
  return v;
}

mgos_zvar_t *mgos_zvar_dic_bool_set(mgos_zvar_t *v, const char *name, bool val) {
  mgos_zvar_bool_set(mg_zvar_dic_get(v, name, true), val);
  return v;
}

mgos_zvar_t *mgos_zvar_dic_str_set(mgos_zvar_t *v, const char *name, const char *val) {
  mgos_zvar_str_set(mg_zvar_dic_get(v, name, true), val);
  return v;
}

long mgos_zvar_dic_bigint_get(mgos_zvar_t *v, const char *name) {
  return mgos_zvar_bigint_get((mgos_zvar_is_dic(v) ? mg_zvar_dic_get(v, name, false) : NULL));
}

double mgos_zvar_dic_decimal_get(mgos_zvar_t *v, const char *name) {
  return mgos_zvar_decimal_get((mgos_zvar_is_dic(v) ? mg_zvar_dic_get(v, name, false) : NULL));
}

bool mgos_zvar_dic_bool_get(mgos_zvar_t *v, const char *name) {
  return mgos_zvar_bool_get((mgos_zvar_is_dic(v) ? mg_zvar_dic_get(v, name, false) : NULL));
}

const char *mgos_zvar_dic_str_get(mgos_zvar_t *v, const char *name) {
  return mgos_zvar_str_get((mgos_zvar_is_dic(v) ? mg_zvar_dic_get(v, name, false) : NULL));
}

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */
