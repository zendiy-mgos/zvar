let ZenVar = {
  _tof: ffi('int mgos_zvar_type(void *)'),
  _bs: ffi('void* mgos_zvar_bool_set(void *, bool)'),
  _bg: ffi('bool mgos_zvar_bool_get(void *)'),
  _is: ffi('void* mgos_zvar_bigint_set(void *, int)'),
  _ig: ffi('long mgos_zvar_bigint_get(void *)'),
  _ds: ffi('void* mgos_zvar_decimal_set(void *, double)'),
  _dg: ffi('double mgos_zvar_decimal_get(void *)'),
  _isnav: ffi('bool mgos_zvar_is_nav(void *)'),
  
  TYPE: {
    UNKNOWN: 0,
    ARRAY: 1,
    BOOL: 2,
    INT: 3,
    DOUBLE: 4
  },
  
  typeOf: function(x) {
    return ZenVar._tof(x);
  },
  
  isNaV: function(x) {
    return ZenVar._isnav(x);
  },
  
  bool: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._bg(x);
    } else {
      // set
      return ZenVar._bs(x, val);
    }
  },
  
  int: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._ig(x);
    } else {
      // set
      return ZenVar._is(x, val);
    }
  },
  
  double: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._dg(x);
    } else {
      // set
      return ZenVar._ds(x, val);
    }
  },
};
