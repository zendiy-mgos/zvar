#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "mgos_zvar.h"

#if MGOS_ZVAR_HAVE_JSON
#include "mgos_zvar_json.h"
#endif

#if MGOS_ZVAR_HAVE_DIC
#include "mgos_zvar_dic.h"
#endif

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

#if MGOS_ZVAR_HAVE_DIC
struct mg_zvar_dic_key {
  const char* name;
  mgos_zvar_t var;
  mgos_zvar_t next_var;
  mgos_zvar_t prev_var;
};

struct mg_zvar_dic_head {
  mgos_zvar_t var;
  int count;
};
#endif

union mgos_zvar_value {
   long l;
   double d;
   bool b;
   char *s;
   #if MGOS_ZVAR_HAVE_DIC
   struct mg_zvar_dic_head dic_head;
   #endif
};

struct mgos_zvariant {
  enum mgos_zvar_type type; 
  union mgos_zvar_value value;
  size_t v_size;
  char changed;
  #if MGOS_ZVAR_HAVE_DIC
  struct mg_zvar_dic_key *key;
  #endif
};

#if MGOS_ZVAR_HAVE_JSON
struct mg_zvar_json_walk_cb_arg {
  mgos_zvar_t var;
  char ret;
};
#endif

#if MGOS_ZVAR_HAVE_DIC

mgos_zvar_t mg_zvar_dic_get_root(mgos_zvar_t var) {
  if (!var) return NULL;
  if (var->key) return mg_zvar_dic_get_root(var->key->prev_var);
  return var;
}

mgos_zvar_t mg_zvar_dic_get_parent(mgos_zvar_t var, bool root) {
  if (!var) return NULL;
  mgos_zvar_t p = (var->key ? var->key->prev_var : NULL);
  while (p && !mgos_zvar_is_dic(p)) {
    p = (p->key ? p->key->prev_var : NULL);
  }
  return p;
}

#endif

void mg_zvar_close(mgos_zvar_t var) {
  if (var) {
    #if MGOS_ZVAR_HAVE_DIC
    if (mgos_zvar_is_dic(var)) mgos_zvar_remove_keys(var);
    #endif
    if (mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_STR) {
      free(var->value.s);
      var->value.s = NULL;
    }
    memset(&var->value, 0, sizeof(union mgos_zvar_value));
    var->v_size = 0;
  }
}

mgos_zvar_t mg_zvar_set_type(mgos_zvar_t var, enum mgos_zvar_type t) {
  if (var) var->type = t;
  return var;
}

void mg_zvar_set_changed(mgos_zvar_t var) {
  if (!var) return;
  #if MGOS_ZVAR_HAVE_DIC
  mgos_zvar_t parent = mg_zvar_dic_get_parent(var, false);
  if (parent != var) { 
    mg_zvar_set_changed(parent);
  }
  #endif
  var->changed = 1;
}

#if MGOS_ZVAR_HAVE_DIC

mgos_zvar_t mg_zvar_dic_ensure(mgos_zvar_t var, bool clear) {
  if (var) {
    if (mgos_zvar_is_dic(var)) {
      if (clear) mgos_zvar_remove_keys(var);
    } else {
      mg_zvar_close(var);
      mg_zvar_set_type(var, MGOS_ZVAR_TYPE_DIC);
    }
  }
  return var;
}

bool mg_zvar_dic_add(mgos_zvar_t root, const char *key_name, size_t key_len, mgos_zvar_t var) {
  if (!mg_zvar_dic_ensure(root, false) || !var || var->key || !key_name) return false;

  mgos_zvar_t last_var = root;
  mgos_zvar_t v = root->value.dic_head.var; 
  while (v && v->key) {
    last_var = v;
    v = v->key->next_var;
  };
 
  // init the new key
  var->key = calloc(1, sizeof(struct mg_zvar_dic_key));
  if (key_len == -1) {
    var->key->name = strdup(key_name);
  } else {
    var->key->name = malloc(key_len + 1);
    strncpy((char *)var->key->name, key_name, key_len);
    ((char *)var->key->name)[key_len] = '\0';
  }
  var->key->var = var;
  
  // attach the new var as first var
  var->key->prev_var = last_var;
  if (last_var == root) {
    last_var->value.dic_head.var = var;
  } else {
    last_var->key->next_var = var;
  }

  // increase dic length
  ++root->value.dic_head.count;
  // set dictionary as changed
  mg_zvar_set_changed(root);

  return true;
}

mgos_zvar_t mg_zvar_dic_get(mgos_zvar_t root, const char *key_name, size_t key_len, bool create) {
  if (!mg_zvar_dic_ensure(root, false) || !key_name) return NULL;

  mgos_zvar_t var = root->value.dic_head.var;
  if (key_len == -1) key_len = strlen(key_name);
  while (var) {
    if (strncmp(var->key->name, key_name, key_len) == 0 && key_len == strlen(var->key->name)) return var;
    var = var->key->next_var;
  }

  if (create) {
    // not found...create a new variant
    mgos_zvar_t var = mgos_zvar_new();
    if (mg_zvar_dic_add(root, key_name, key_len, var)) return var;
    mgos_zvar_free(var);
  }

  return NULL;
}

bool mg_zvar_dic_are_equal(mgos_zvarc_t var1, mgos_zvarc_t var2) {
  if (!mgos_zvar_is_dic(var1) || !mgos_zvar_is_dic(var2)) return false;
  if (mgos_zvar_length(var1) != mgos_zvar_length(var2)) return false;

  mgos_zvar_t var = var1->value.dic_head.var;
  while(var) {
    mgos_zvar_t value = mg_zvar_dic_get((mgos_zvar_t)var2, var->key->name, strlen(var->key->name), false);
    if (!value) return false;
    if (mgos_zvar_cmp(value, var) != 0) return false;
    var = var->key->next_var;
  }

  return true;
}

bool mg_zvar_dic_copy(mgos_zvarc_t src, mgos_zvar_t dest, bool del_unmatch) {
  if (!mgos_zvar_is_dic(src)) return false;
  if (!mg_zvar_dic_ensure(dest, false)) return false;

  // Sync existing keys and add new keys
  mgos_zvar_t value;
  mgos_zvar_t var = src->value.dic_head.var;
  while(var) {
    value = mg_zvar_dic_get(dest, var->key->name, strlen(var->key->name), true);
    if (mgos_zvar_cmp(value, var) != 0) {
      mgos_zvar_copy(var, value);
    }
    var = var->key->next_var;
  }
  
  if (del_unmatch) {
    // Remove not existing keys 
    var = dest->value.dic_head.var;
    while(var) {
      if (!mg_zvar_dic_get((mgos_zvar_t)src, var->key->name, strlen(var->key->name), false)) {
        value = var;
        var = var->key->next_var; 
        mgos_zvar_free(value);
      } else {
        var = var->key->next_var;
      }
    }
  }
  
  return true;
}

#endif //MGOS_ZVAR_HAVE_DIC

#if MGOS_ZVAR_HAVE_JSON

void mg_zvar_json_walk_cb(void *callback_data,
                          const char *name, size_t name_len,
                          const char *path,
                          const struct json_token *token) {
  struct mg_zvar_json_walk_cb_arg *arg = (struct mg_zvar_json_walk_cb_arg *)callback_data;
  if (!arg || (arg->ret != 0)) return;
  
  mgos_zvar_t new_item;
  if (token->type == JSON_TYPE_STRING || token->type == JSON_TYPE_NUMBER ||
      token->type == JSON_TYPE_FALSE  || token->type == JSON_TYPE_TRUE ||
      token->type == JSON_TYPE_NULL   || token->type == JSON_TYPE_OBJECT_START) {
    #if MGOS_ZVAR_HAVE_DIC
    new_item = (token->type == JSON_TYPE_OBJECT_START ? mgos_zvar_new_dic() : mgos_zvar_new()); 
    if (arg->var && name) {
      mg_zvar_dic_add(arg->var, name, name_len, new_item);
    }
    if (token->type == JSON_TYPE_OBJECT_START || !arg->var) arg->var = new_item;
    #else
    arg->var = new_item = mgos_zvar_new();
    #endif
  }
      
  switch(token->type) {
    case JSON_TYPE_NULL:
      break;
    case JSON_TYPE_NUMBER:
      if (memchr(token->ptr, '.', token->len) != NULL) {
        double value;
        json_scanf(token->ptr, token->len, "%lf", &value);
        mgos_zvar_set_decimal(new_item, value);
      } else {
        long value;
        json_scanf(token->ptr, token->len, "%ld", &value);
        mgos_zvar_set_integer(new_item, value);
      }
      break;
    case JSON_TYPE_TRUE:
      mgos_zvar_set_bool(new_item, true);
      break;
    case JSON_TYPE_FALSE:
      mgos_zvar_set_bool(new_item, false);
      break;
    case JSON_TYPE_STRING:
      mgos_zvar_set_nstr(new_item, token->ptr, token->len);
      break;
    #if MGOS_ZVAR_HAVE_DIC
    case JSON_TYPE_OBJECT_START:
      // nothing to do
      break;
    case JSON_TYPE_OBJECT_END: {
        mgos_zvar_t parent = mg_zvar_dic_get_parent(arg->var, false);
        if (parent) arg->var = parent;
        break;
      }
    #else
    case JSON_TYPE_OBJECT_START:
      arg->ret = 1; // stop walking
      break;
    #endif
    default:
      arg->ret = -1; // stop walking + error
      break;
  };
}

#endif // MGOS_ZVAR_HAVE_JSON

bool mgos_zvar_is_null(mgos_zvarc_t var) {
  return (mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_NULL);
}

mgos_zvar_t mgos_zvar_new() {
  return (mgos_zvar_t)calloc(1, sizeof(struct mgos_zvariant));
}

mgos_zvar_t mgos_zvar_new_integer(long value) {
  mgos_zvar_t var = mgos_zvar_new();
  mgos_zvar_set_integer(var, value);
  mgos_zvar_set_unchanged(var);
  return var;
}

mgos_zvar_t mgos_zvar_new_bool(bool value) {
  mgos_zvar_t var = mgos_zvar_new();
  mgos_zvar_set_bool(var, value);
  mgos_zvar_set_unchanged(var);
  return var;
}

mgos_zvar_t mgos_zvar_new_decimal(double value) {
  mgos_zvar_t var = mgos_zvar_new();
  mgos_zvar_set_decimal(var, value);
  mgos_zvar_set_unchanged(var);
  return var;
}

mgos_zvar_t mgos_zvar_new_str(const char *value) {
  mgos_zvar_t var = mgos_zvar_new();
  mgos_zvar_set_str(var, value);
  mgos_zvar_set_unchanged(var);
  return var;
}

enum mgos_zvar_type mgos_zvar_get_type(mgos_zvarc_t var) {
  return (var ? var->type : MGOS_ZVAR_TYPE_NULL);
}

void mgos_zvar_set_null(mgos_zvar_t var) {
  if (var && (mgos_zvar_get_type(var) != MGOS_ZVAR_TYPE_NULL)) {
    #if MGOS_ZVAR_HAVE_DIC
    if (mgos_zvar_is_dic(var)) {
      mgos_zvar_remove_keys(var);
    }
    #endif
    mg_zvar_close(var);
    mg_zvar_set_type(var, MGOS_ZVAR_TYPE_NULL);
    mg_zvar_set_changed(var);
  }
}

int mgos_zvar_cmp(mgos_zvarc_t var1, mgos_zvarc_t var2) {
  if (var1 == NULL && var2 == NULL) return 0; // both NULL
  if (var1 == NULL && var2 != NULL) return -1; // var1 < var2
  if (var1 != NULL && var2 == NULL) return 1; // var1 > var2
  if (var1 == var2) return 0; // comparing the same instance
  
   #if MGOS_ZVAR_HAVE_DIC
  if(mgos_zvar_is_dic(var1) || mgos_zvar_is_dic(var2)) {
    return (mg_zvar_dic_are_equal(var1, var2) ? 0 : -1);
  }
  #endif

  enum mgos_zvar_type t1 = mgos_zvar_get_type(var1);
  enum mgos_zvar_type t2 = mgos_zvar_get_type(var2);
  
  if(t1 != t2) {
    if (t1 != MGOS_ZVAR_TYPE_INTEGER && t1 != MGOS_ZVAR_TYPE_DECIMAL ||
        t1 != MGOS_ZVAR_TYPE_INTEGER && t1 != MGOS_ZVAR_TYPE_DECIMAL) {
      return INT_MAX;       
    }
  }
  
  switch(t1) {
    case MGOS_ZVAR_TYPE_INTEGER:
      if (t2 == MGOS_ZVAR_TYPE_INTEGER)
        return (var1->value.l < var2->value.l ? -1 : (var1->value.l > var2->value.l ? 1 : 0));
      else
        return (var1->value.l < var2->value.d ? -1 : (var1->value.l > var2->value.d ? 1 : 0));
    case MGOS_ZVAR_TYPE_DECIMAL:
      if (t2 == MGOS_ZVAR_TYPE_DECIMAL)
        return (var1->value.d < var2->value.d ? -1 : (var1->value.d > var2->value.d ? 1 : 0));
      else
        return (var1->value.d < var2->value.l ? -1 : (var1->value.d > var2->value.l ? 1 : 0));
    case MGOS_ZVAR_TYPE_BOOL:
      return (var1->value.b < var2->value.b ? -1 : (var1->value.b > var2->value.b ? 1 : 0));
    case MGOS_ZVAR_TYPE_STR:
      return strcmp(var1->value.s, var2->value.s);
    case MGOS_ZVAR_TYPE_NULL:
      return 0;
  };

  return INT_MAX; //error
}

bool mgos_zvar_copy(mgos_zvarc_t src_var, mgos_zvar_t dest_var) {
  if (!src_var || !dest_var) return false;
  if (src_var == dest_var) return true; // coping the same instance
  
  #if MGOS_ZVAR_HAVE_DIC
  if (mgos_zvar_is_dic(src_var)) {
    return mg_zvar_dic_copy(src_var, dest_var, true);
  }
  #endif

  switch(mgos_zvar_get_type(src_var)) {
    case MGOS_ZVAR_TYPE_INTEGER:
      mgos_zvar_set_integer(dest_var, src_var->value.l);
      break;
    case MGOS_ZVAR_TYPE_DECIMAL:
      mgos_zvar_set_decimal(dest_var, src_var->value.d);
      break;
    case MGOS_ZVAR_TYPE_BOOL:
      mgos_zvar_set_bool(dest_var, src_var->value.b);
      break;
    case MGOS_ZVAR_TYPE_STR:
      mgos_zvar_set_str(dest_var, (const char *)src_var->value.s);
      break;
    case MGOS_ZVAR_TYPE_NULL:
      mgos_zvar_set_null(dest_var);
      break;
    default:
      return false;
  };

  return true;
}

void mgos_zvar_set_integer(mgos_zvar_t var, long value) {
  if (var) {
    if (mgos_zvar_get_type(var) != MGOS_ZVAR_TYPE_INTEGER) {
      mg_zvar_close(var);
      mg_zvar_set_type(var, MGOS_ZVAR_TYPE_INTEGER);
      var->v_size = sizeof(long);
      mg_zvar_set_changed(var);
    } else if (value != var->value.l) {
      mg_zvar_set_changed(var);
    }
    var->value.l = value;
  }
}

void mgos_zvar_set_bool(mgos_zvar_t var, bool value) {
  if (var) {
    if (mgos_zvar_get_type(var) != MGOS_ZVAR_TYPE_BOOL) {
      mg_zvar_close(var);
      mg_zvar_set_type(var, MGOS_ZVAR_TYPE_BOOL);
      var->v_size = sizeof(bool);
      mg_zvar_set_changed(var);
    } else if (var->value.b != value) {
      mg_zvar_set_changed(var);
    }
    var->value.b = value;
  }
}

void mgos_zvar_set_decimal(mgos_zvar_t var, double value) {
  if (var) {
    if (mgos_zvar_get_type(var) != MGOS_ZVAR_TYPE_DECIMAL) {
      mg_zvar_close(var);
      mg_zvar_set_type(var, MGOS_ZVAR_TYPE_DECIMAL);
      var->v_size = sizeof(double);
      mg_zvar_set_changed(var);
    } else if (var->value.d != value) {
      mg_zvar_set_changed(var);
    }
    var->value.d = value;
  }
}

void mgos_zvar_set_nstr(mgos_zvar_t var, const char *value, size_t value_len) {
  if (!var) return;
  
  if (!value) {
    mgos_zvar_set_null(var);
    return;
  }
  
  if (value_len == -1) value_len = strlen(value);
  
  if (mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_STR && value_len < var->v_size) {
    // re-use previously allocated buffer
    if (strncmp(var->value.s, value, value_len) != 0 || value_len != strlen(var->value.s)) {
      strncpy(var->value.s, value, value_len);
      var->value.s[value_len] = '\0';
      mg_zvar_set_changed(var);
    }
    return;
  }

  mg_zvar_close(var);
  mg_zvar_set_type(var, MGOS_ZVAR_TYPE_STR);
  if (value) {
    var->value.s = strndup(value, value_len);
    var->v_size = (value_len + 1);
  }
  mg_zvar_set_changed(var);
}

void mgos_zvar_set_str(mgos_zvar_t var, const char *value) {
  mgos_zvar_set_nstr(var, value, -1);
}

void mgos_zvar_set_unchanged(mgos_zvar_t var) {
  if (!var) return;
  #if MGOS_ZVAR_HAVE_DIC
  if (mgos_zvar_is_dic(var)) {
    mgos_zvar_t v = var->value.dic_head.var;
    while(v) {
      mgos_zvar_set_unchanged(v);
      v = v->key->next_var;
    }
  }
  #endif
  var->changed = 0;
}

bool mgos_zvar_is_changed(mgos_zvarc_t var) {
  return (var ? (var->changed == 1) : false);
}

long mgos_zvar_get_integer(mgos_zvarc_t var) {
  switch(mgos_zvar_get_type(var)) {
    case MGOS_ZVAR_TYPE_INTEGER:
      return var->value.l;
    case MGOS_ZVAR_TYPE_DECIMAL:
      return (long)var->value.d;
    case MGOS_ZVAR_TYPE_BOOL:
      return (long)var->value.b;
    default:
      return 0;
  }
}

bool mgos_zvar_get_bool(mgos_zvarc_t var) {
  switch(mgos_zvar_get_type(var)) {
    case MGOS_ZVAR_TYPE_BOOL:
      return var->value.b;
    case MGOS_ZVAR_TYPE_INTEGER:
      return (var->value.l != 0);
    case MGOS_ZVAR_TYPE_DECIMAL:
      return (var->value.d != 0);
    case MGOS_ZVAR_TYPE_STR:
      return (var->value.s == NULL ? false : (strlen(var->value.s) > 0));
    default:
      return false;
  }
}

double mgos_zvar_get_decimal(mgos_zvarc_t var) {
  switch(mgos_zvar_get_type(var)) {
    case MGOS_ZVAR_TYPE_DECIMAL:
      return var->value.d;
    case MGOS_ZVAR_TYPE_INTEGER:
      return (double)var->value.l;
    default:
      return 0.0;
  }
}

const char *mgos_zvar_get_str(mgos_zvarc_t var) {
  return ((mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_STR) ? var->value.s : NULL);
}

#if MGOS_ZVAR_HAVE_JSON

int json_printf_zvar(struct json_out *out, va_list *ap) {
  mgos_zvarc_t var = va_arg(*ap, void *);
  
  int len = 0;
  #if MGOS_ZVAR_HAVE_DIC
  if (var->key) len += json_printf(out, "%Q:", var->key->name);
  if (mgos_zvar_is_dic(var)) {
    len += json_printf(out, "{");
    mgos_zvar_t v = var->value.dic_head.var;
    while (v) {
      len += json_printf(out, "%M", json_printf_zvar, v);
      v = v->key->next_var;
      if (v) len += json_printf(out, ",");
    }
    len += json_printf(out, "}");
  }
  #endif

  switch(mgos_zvar_get_type(var)) {
    case MGOS_ZVAR_TYPE_INTEGER:
      len += json_printf(out, "%ld", var->value.l);
      break;
    case MGOS_ZVAR_TYPE_DECIMAL:
      len += json_printf(out, "%lf", var->value.d);
      break;
    case MGOS_ZVAR_TYPE_BOOL:
      len += json_printf(out, "%B", var->value.b);
      break;
    case MGOS_ZVAR_TYPE_STR:
      len += json_printf(out, "%Q", var->value.s);
      break;
    case MGOS_ZVAR_TYPE_NULL:
      len += json_printf(out, "%Q", NULL);
      break;
  };

  return len;
}

mgos_zvar_t mgos_zvar_json_scanf(const char *json) {
  mgos_zvar_t var = NULL;
  if (json) {
    struct mg_zvar_json_walk_cb_arg arg = { .var = NULL, .ret = 0 };
    if ((json_walk(json, strlen(json), mg_zvar_json_walk_cb, &arg) > 0) && (arg.ret != -1)) {
      #if MGOS_ZVAR_HAVE_DIC
      var = mg_zvar_dic_get_root(arg.var);
      #else
      var = arg.var;
      #endif
    } else {
      mgos_zvar_free(arg.var);
    }
  }
  mgos_zvar_set_unchanged(var);
  return var;
} 

#endif // MGOS_ZVAR_HAVE_JSON

void mgos_zvar_free(mgos_zvar_t var) {
  if (!var) return;
  
  #if MGOS_ZVAR_HAVE_DIC
  bool is_dic = mgos_zvar_is_dic(var);
  if (is_dic) {
    while (var->value.dic_head.var) {
      mgos_zvar_free(var->value.dic_head.var);
    }
  }
  #endif

  #if MGOS_ZVAR_HAVE_DIC
  struct mg_zvar_dic_key *key = var->key;
  if (key) {
    mgos_zvar_t parent = mg_zvar_dic_get_parent(var, false);
    if (parent) {
      --parent->value.dic_head.count;  // decrease dic length
      mg_zvar_set_changed(parent); // set dic as changed
    }
  
    // start removing key...
    var->key = NULL;
    
    if (key->prev_var) {
      if (mgos_zvar_is_dic(key->prev_var)) {
        key->prev_var->value.dic_head.var = key->next_var;
      } else {
        key->prev_var->key->next_var = key->next_var;
      }
    }
    
    if (key->next_var) {
      key->next_var->key->prev_var = key->prev_var;
    }
    
    free((char *)key->name);
    free(key);
  }
  #endif
  
  mg_zvar_close(var);
  free(var);
}

int mgos_zvar_length(mgos_zvarc_t var) {
  #if MGOS_ZVAR_HAVE_DIC
  if (mgos_zvar_is_dic(var)) {
    return var->value.dic_head.count;
  }
  #endif
  if (mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_STR) {
    return var->value.s ? strlen(var->value.s) : 0;
  }
  return 0;
}

#if MGOS_ZVAR_HAVE_DIC

mgos_zvar_t mgos_zvar_new_dic() {
  return mg_zvar_dic_ensure(mgos_zvar_new(), false);
}

bool mgos_zvar_merge(mgos_zvarc_t src_var, mgos_zvar_t dest_var) {
  if (src_var == dest_var) return true; // merging the same instance
  if (mgos_zvar_is_dic(src_var)) {
    return  mg_zvar_dic_copy(src_var, dest_var, false);
  } else {
    return mgos_zvar_copy(src_var, dest_var);
  }
}

bool mgos_zvar_is_dic(mgos_zvarc_t var) {
  return (mgos_zvar_get_type(var) == MGOS_ZVAR_TYPE_DIC);
}

void mgos_zvar_remove_keys(mgos_zvar_t var) {
  if (mgos_zvar_is_dic(var)) {
    while (var->value.dic_head.var) {
      mgos_zvar_free(var->value.dic_head.var);
    }
  }
}

void mgos_zvar_remove_key(mgos_zvar_t var, const char *key) {
  mgos_zvar_free(mg_zvar_dic_get(var, key, (key ? strlen(key) : 0), false));
}

bool mgos_zvar_add_key(mgos_zvar_t var, const char *key, mgos_zvar_t val) {
  if (mg_zvar_dic_get(var, key, -1, false) == NULL) {
    if (mg_zvar_dic_add(var, key, -1, val)) { return true; }
  } 
  return false;
}

mgos_zvar_t mgos_zvar_get_key(mgos_zvar_t var, const char *key) {
  return (mgos_zvar_t)mgos_zvar_get_ckey(var, key);
}

bool mgos_zvar_try_get_key(mgos_zvar_t var, const char *key, mgos_zvar_t *out) {
  return mgos_zvar_try_get_ckey(var, key, (mgos_zvarc_t *)out);
}

bool mgos_zvar_get_next_key(mgos_zvar_enum_t *key_enum, mgos_zvar_t *out, const char **key_name) {
  return mgos_zvarc_get_next_key((mgos_zvarc_enum_t *)key_enum, (mgos_zvarc_t *)out, key_name);
}

mgos_zvarc_t mgos_zvar_get_ckey(mgos_zvarc_t var, const char *key) {
  return mg_zvar_dic_get((mgos_zvar_t)var, key, (key ? strlen(key) : 0), false);
}

bool mgos_zvar_try_get_ckey(mgos_zvarc_t var, const char *key, mgos_zvarc_t *out) {
  mgos_zvarc_t ret = mgos_zvar_get_ckey(var, key);
  if (out) *out = ret;
  return (ret != NULL);
}

bool mgos_zvarc_get_next_key(mgos_zvarc_enum_t *key_enum, mgos_zvarc_t *out, const char **key_name) {
  if (key_name) *key_name = NULL;
  if (out) *out = NULL;
  if (!key_enum || (*key_enum == NULL)) return false; // no next

  mgos_zvar_t var = (mgos_zvar_t)*key_enum;

  if (mgos_zvar_is_dic(var)) {
    var = var->value.dic_head.var;
  }
  
  if (!var || !var->key) return false;
  
  if (out) *out  = var;
  if (key_name) *key_name = var->key->name;
  *key_enum = (mgos_zvar_enum_t)var->key->next_var;
  return true;
}

bool mgos_zvar_has_key(mgos_zvarc_t var, const char *key) {
  return (mg_zvar_dic_get((mgos_zvar_t)var, key, (key ? strlen(key) : 0), false) != NULL);
}

mgos_zvar_enum_t mgos_zvar_get_keys(mgos_zvar_t var) {
  return (mgos_zvar_enum_t)var;
}

mgos_zvar_enum_t mgos_zvarc_get_keys(mgos_zvarc_t var) {
  return (mgos_zvarc_enum_t)var;
}

#endif //MGOS_ZVAR_HAVE_DIC

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */

bool mgos_zvar_init() {
  return true;
}