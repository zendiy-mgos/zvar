let ZenVar = {
  _tof: ffi('int mgos_zvar_type(void *)'),
  _bs: ffi('void* mgos_zvar_bool_set(void *, bool)'),
  _bg: ffi('bool mgos_zvar_bool_get(void *)'),
  _is: ffi('void* mgos_zvar_bigint_set(void *, long)'),
  _ig: ffi('long mgos_zvar_bigint_get(void *)'),
  _ds: ffi('void* mgos_zvar_decimal_set(void *, double)'),
  _dg: ffi('double mgos_zvar_decimal_get(void *)'),
  _ss: ffi('void* mgos_zvar_str_set(void *, char *)'),
  _sg: ffi('char *mgos_zvar_str_get(void *)'),

  _dbs: ffi('void *mgos_zvar_dic_bool_set(void *, char *, bool)'),
  _dbg: ffi('bool mgos_zvar_dic_bool_get(void *, char *)'),
  _dis: ffi('void *mgos_zvar_dic_bigint_set(void *, char *, long)'),
  _dig: ffi('long mgos_zvar_dic_bigint_get(void *, char *)'),
  _dds: ffi('void *mgos_zvar_dic_decimal_set(void *, char *, double)'),
  _ddg: ffi('double mgos_zvar_dic_decimal_get(void *, char *)'),
  _dss: ffi('void *mgos_zvar_dic_str_set(void *, char *, char *)'),
  _dsg: ffi('const char *mgos_zvar_dic_str_get(void *, char *)'),
  _dcnt: ffi('int mgos_zvar_dic_count(void *)'),
  _dcls: ffi('void mgos_zvar_dic_clear(void *)'),

  _isnav: ffi('bool mgos_zvar_is_nav(void *)'),
  _isdic: ffi('bool mgos_zvar_is_dic(void *)'),
  
  TYPE: {
    UNKNOWN: 0,     //00000
    DICTIONARY: 1,  //00001
    BOOL: 2,        //00010
    BIGINT: 4,      //00100
    DECIMAL: 8,     //01000
    STR: 16         //10000
  },
  
  typeOf: function(x) {
    return ZenVar._tof(x);
  },
  isNaV: function(x) {
    return ZenVar._isnav(x);
  },
  isDictionary: function(x) {
    return ZenVar._isdic(x);
  },
  
  bool: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._bg(x);
    } else {
      // set
      ZenVar._bs(x, val);
      return this;
    }
  },
  
  int: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._ig(x);
    } else {
      // set
      ZenVar._is(x, val);
      return this;
    }
  },
  
  decimal: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._dg(x);
    } else {
      // set
      ZenVar._ds(x, val);
      return this;
    }
  },

  string: function(x, val) {
    if (val === undefined) {
      // get
      return ZenVar._sg(x);
    } else {
      // set
      ZenVar._ss(x, val);
      return this;
    }
  },

  dictionary: function(x, key) {
    this._dic._key = key;
    this._dic._var = x;
    this._parent = this;
    return this._dic;
  },

  _dic: {
    _key: null,
    _var: null,
    _parent: null,

    count: function() {
      return ZenVar._dcnt(this._var);
    },

    clear: function() {
      ZenVar._dcls(this._var);
      return this._parent;
    },

    bool: function(val) {
      if (val === undefined) {
        // get
        return ZenVar._dbg(this._var, this._key);
      } else {
        // set
        ZenVar._dbs(this._var, this._key, val);
        return this._parent;
      }
    },
    
    int: function(val) {
      if (val === undefined) {
        // get
        return ZenVar._dig(this._var, this._key);
      } else {
        // set
        ZenVar._dis(this._var, this._key, val);
        return this._parent;
      }
    },
    
    decimal: function(val) {
      if (val === undefined) {
        // get
        return ZenVar._ddg(this._var, this._key);
      } else {
        // set
        ZenVar._dds(this._var, this._key, val);
        return this._parent;
      }
    },
  
    string: function(val) {
      if (val === undefined) {
        // get
        return ZenVar._dsg(this._var, this._key);
      } else {
        // set
        ZenVar._dss(this._var, this._key, val);
        return this._parent;
      }
    },
  },
};
