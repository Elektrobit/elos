// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"

#include <stdio.h>

MOCK_FUNC_VAR_NEW(json_object_new_object);
struct json_object *MOCK_FUNC_WRAP(json_object_new_object)() {
    if (MOCK_IS_ACTIVE(json_object_new_object)) {
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_object)();
}

MOCK_FUNC_VAR_NEW(json_object_get);
struct json_object *MOCK_FUNC_WRAP(json_object_get)(struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get)) {
        check_expected_ptr(obj);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_get)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_put);
int MOCK_FUNC_WRAP(json_object_put)(struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_put)) {
        check_expected_ptr(obj);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_put)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_is_type);
int MOCK_FUNC_WRAP(json_object_is_type)(const struct json_object *obj, enum json_type type) {
    if (MOCK_IS_ACTIVE(json_object_is_type)) {
        check_expected_ptr(obj);
        check_expected(type);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_is_type)(obj, type);
}

MOCK_FUNC_VAR_NEW(json_object_get_type);
enum json_type MOCK_FUNC_WRAP(json_object_get_type)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_type)) {
        check_expected_ptr(obj);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_get_type)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_to_json_string);
const char *MOCK_FUNC_WRAP(json_object_to_json_string)(struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_to_json_string)) {
        check_expected_ptr(obj);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(json_object_to_json_string)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_to_json_string_ext);
const char *MOCK_FUNC_WRAP(json_object_to_json_string_ext)(struct json_object *obj, int flags) {
    if (MOCK_IS_ACTIVE(json_object_to_json_string_ext)) {
        check_expected_ptr(obj);
        check_expected(flags);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(json_object_to_json_string_ext)(obj, flags);
}

MOCK_FUNC_VAR_NEW(json_object_to_json_string_length);
const char *MOCK_FUNC_WRAP(json_object_to_json_string_length)(struct json_object *obj, int flags, size_t *length) {
    if (MOCK_IS_ACTIVE(json_object_to_json_string_length)) {
        check_expected_ptr(obj);
        check_expected(flags);
        check_expected_ptr(length);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(json_object_to_json_string_length)(obj, flags, length);
}

MOCK_FUNC_VAR_NEW(json_object_get_userdata);
void *MOCK_FUNC_WRAP(json_object_get_userdata)(json_object *jso) {
    if (MOCK_IS_ACTIVE(json_object_get_userdata)) {
        check_expected_ptr(jso);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(json_object_get_userdata)(jso);
}

MOCK_FUNC_VAR_NEW(json_object_set_userdata);
void MOCK_FUNC_WRAP(json_object_set_userdata)(json_object *jso, void *userdata, json_object_delete_fn *user_delete) {
    if (MOCK_IS_ACTIVE(json_object_set_userdata)) {
        check_expected_ptr(jso);
        check_expected_ptr(userdata);
        check_expected_ptr(user_delete);
    }
    MOCK_FUNC_REAL(json_object_set_userdata)(jso, userdata, user_delete);
}

MOCK_FUNC_VAR_NEW(json_object_set_serializer);
void MOCK_FUNC_WRAP(json_object_set_serializer)(json_object *jso, json_object_to_json_string_fn *to_string_func,
                                                void *userdata, json_object_delete_fn *user_delete) {
    if (MOCK_IS_ACTIVE(json_object_set_serializer)) {
        check_expected_ptr(jso);
        check_expected_ptr(to_string_func);
        check_expected_ptr(userdata);
        check_expected_ptr(user_delete);
    }
    MOCK_FUNC_REAL(json_object_set_serializer)(jso, to_string_func, userdata, user_delete);
}

MOCK_FUNC_VAR_NEW(json_object_get_object);
struct lh_table *MOCK_FUNC_WRAP(json_object_get_object)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_object)) {
        check_expected_ptr(obj);
        return mock_ptr_type(lh_table *);
    }
    return MOCK_FUNC_REAL(json_object_get_object)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_object_length);
int MOCK_FUNC_WRAP(json_object_object_length)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_object_length)) {
        check_expected_ptr(obj);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_object_length)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_object_add);
int MOCK_FUNC_WRAP(json_object_object_add)(struct json_object *obj, const char *key, struct json_object *val) {
    if (MOCK_IS_ACTIVE(json_object_object_add)) {
        check_expected_ptr(obj);
        check_expected_ptr(key);
        check_expected_ptr(val);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_object_add)(obj, key, val);
}

MOCK_FUNC_VAR_NEW(json_object_object_add_ex);
int MOCK_FUNC_WRAP(json_object_object_add_ex)(struct json_object *obj, const char *const key,
                                              struct json_object *const val, const unsigned opts) {
    if (MOCK_IS_ACTIVE(json_object_object_add_ex)) {
        check_expected_ptr(obj);
        check_expected_ptr(key);
        check_expected_ptr(val);
        check_expected(opts);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_object_add_ex)(obj, key, val, opts);
}

MOCK_FUNC_VAR_NEW(json_object_object_get);
struct json_object *MOCK_FUNC_WRAP(json_object_object_get)(const struct json_object *obj, const char *key) {
    if (MOCK_IS_ACTIVE(json_object_object_get)) {
        check_expected_ptr(obj);
        check_expected_ptr(key);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_object_get)(obj, key);
}

MOCK_FUNC_VAR_NEW(json_object_object_get_ex);
json_bool MOCK_FUNC_WRAP(json_object_object_get_ex)(const struct json_object *obj, const char *key,
                                                    struct json_object **value) {
    if (MOCK_IS_ACTIVE(json_object_object_get_ex)) {
        check_expected_ptr(obj);
        check_expected_ptr(key);
        check_expected_ptr(value);
        return mock_type(json_bool);
    }
    return MOCK_FUNC_REAL(json_object_object_get_ex)(obj, key, value);
}

MOCK_FUNC_VAR_NEW(json_object_object_del);
void MOCK_FUNC_WRAP(json_object_object_del)(struct json_object *obj, const char *key) {
    if (MOCK_IS_ACTIVE(json_object_object_del)) {
        check_expected_ptr(obj);
        check_expected_ptr(key);
    }
    MOCK_FUNC_REAL(json_object_object_del)(obj, key);
}

MOCK_FUNC_VAR_NEW(json_object_new_array);
struct json_object *MOCK_FUNC_WRAP(json_object_new_array)() {
    if (MOCK_IS_ACTIVE(json_object_new_array)) {
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_array)();
}

MOCK_FUNC_VAR_NEW(json_object_new_array_ext);
struct json_object *MOCK_FUNC_WRAP(json_object_new_array_ext)(int initial_size) {
    if (MOCK_IS_ACTIVE(json_object_new_array_ext)) {
        check_expected(initial_size);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_array_ext)(initial_size);
}

MOCK_FUNC_VAR_NEW(json_object_get_array);
struct array_list *MOCK_FUNC_WRAP(json_object_get_array)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_array)) {
        check_expected_ptr(obj);
        return mock_ptr_type(array_list *);
    }
    return MOCK_FUNC_REAL(json_object_get_array)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_array_length);
size_t MOCK_FUNC_WRAP(json_object_array_length)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_array_length)) {
        check_expected_ptr(obj);
        return mock_type(size_t);
    }
    return MOCK_FUNC_REAL(json_object_array_length)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_array_add);
int MOCK_FUNC_WRAP(json_object_array_add)(struct json_object *obj, struct json_object *val) {
    if (MOCK_IS_ACTIVE(json_object_array_add)) {
        check_expected_ptr(obj);
        check_expected_ptr(val);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_array_add)(obj, val);
}

MOCK_FUNC_VAR_NEW(json_object_array_put_idx);
int MOCK_FUNC_WRAP(json_object_array_put_idx)(struct json_object *obj, size_t idx, struct json_object *val) {
    if (MOCK_IS_ACTIVE(json_object_array_put_idx)) {
        check_expected_ptr(obj);
        check_expected(idx);
        check_expected_ptr(val);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_array_put_idx)(obj, idx, val);
}

MOCK_FUNC_VAR_NEW(json_object_array_get_idx);
struct json_object *MOCK_FUNC_WRAP(json_object_array_get_idx)(const struct json_object *obj, size_t idx) {
    if (MOCK_IS_ACTIVE(json_object_array_get_idx)) {
        check_expected_ptr(obj);
        check_expected(idx);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_array_get_idx)(obj, idx);
}

MOCK_FUNC_VAR_NEW(json_object_array_del_idx);
int MOCK_FUNC_WRAP(json_object_array_del_idx)(struct json_object *obj, size_t idx, size_t count) {
    if (MOCK_IS_ACTIVE(json_object_array_del_idx)) {
        check_expected_ptr(obj);
        check_expected(idx);
        check_expected(count);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_array_del_idx)(obj, idx, count);
}

MOCK_FUNC_VAR_NEW(json_object_array_shrink);
int MOCK_FUNC_WRAP(json_object_array_shrink)(struct json_object *jso, int empty_slots) {
    if (MOCK_IS_ACTIVE(json_object_array_shrink)) {
        check_expected_ptr(jso);
        check_expected(empty_slots);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_array_shrink)(jso, empty_slots);
}

MOCK_FUNC_VAR_NEW(json_object_new_boolean);
struct json_object *MOCK_FUNC_WRAP(json_object_new_boolean)(json_bool b) {
    if (MOCK_IS_ACTIVE(json_object_new_boolean)) {
        check_expected(b);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_boolean)(b);
}

MOCK_FUNC_VAR_NEW(json_object_get_boolean);
json_bool MOCK_FUNC_WRAP(json_object_get_boolean)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_boolean)) {
        check_expected_ptr(obj);
        return mock_ptr_type(json_bool);
    }
    return MOCK_FUNC_REAL(json_object_get_boolean)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_set_boolean);
int MOCK_FUNC_WRAP(json_object_set_boolean)(struct json_object *obj, json_bool new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_boolean)) {
        check_expected_ptr(obj);
        check_expected(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_boolean)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_new_int);
struct json_object *MOCK_FUNC_WRAP(json_object_new_int)(int32_t i) {
    if (MOCK_IS_ACTIVE(json_object_new_int)) {
        check_expected(i);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_int)(i);
}

MOCK_FUNC_VAR_NEW(json_object_new_int64);
struct json_object *MOCK_FUNC_WRAP(json_object_new_int64)(int64_t i) {
    if (MOCK_IS_ACTIVE(json_object_new_int64)) {
        check_expected(i);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_int64)(i);
}

MOCK_FUNC_VAR_NEW(json_object_new_uint64);
struct json_object *MOCK_FUNC_WRAP(json_object_new_uint64)(uint64_t i) {
    if (MOCK_IS_ACTIVE(json_object_new_uint64)) {
        check_expected(i);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_uint64)(i);
}

MOCK_FUNC_VAR_NEW(json_object_get_int);
int MOCK_FUNC_WRAP(json_object_get_int_errno = 0);
int32_t MOCK_FUNC_WRAP(json_object_get_int)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_int)) {
        check_expected_ptr(obj);
        if (MOCK_FUNC_WRAP(json_object_get_int_errno) != 0) {
            errno = MOCK_FUNC_WRAP(json_object_get_int_errno);
            MOCK_FUNC_WRAP(json_object_get_int_errno) = 0;
        }
        return mock_type(int32_t);
    }
    return MOCK_FUNC_REAL(json_object_get_int)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_set_int);
int MOCK_FUNC_WRAP(json_object_set_int)(struct json_object *obj, int new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_int)) {
        check_expected_ptr(obj);
        check_expected(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_int)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_int_inc);
int MOCK_FUNC_WRAP(json_object_int_inc)(struct json_object *obj, int64_t val) {
    if (MOCK_IS_ACTIVE(json_object_int_inc)) {
        check_expected_ptr(obj);
        check_expected(val);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_int_inc)(obj, val);
}

MOCK_FUNC_VAR_NEW(json_object_get_int64);
int MOCK_FUNC_WRAP(json_object_get_int64_errno = 0);
int64_t MOCK_FUNC_WRAP(json_object_get_int64)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_int64)) {
        check_expected_ptr(obj);
        if (MOCK_FUNC_WRAP(json_object_get_int64_errno) != 0) {
            errno = MOCK_FUNC_WRAP(json_object_get_int64_errno);
            MOCK_FUNC_WRAP(json_object_get_int64_errno) = 0;
        }
        return mock_type(int64_t);
    }
    return MOCK_FUNC_REAL(json_object_get_int64)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_get_uint64);
int MOCK_FUNC_WRAP(json_object_get_uint64_errno = 0);
uint64_t MOCK_FUNC_WRAP(json_object_get_uint64)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_uint64)) {
        check_expected_ptr(obj);
        if (MOCK_FUNC_WRAP(json_object_get_uint64_errno) != 0) {
            errno = MOCK_FUNC_WRAP(json_object_get_uint64_errno);
            MOCK_FUNC_WRAP(json_object_get_uint64_errno) = 0;
        }
        return mock_type(uint64_t);
    }
    return MOCK_FUNC_REAL(json_object_get_uint64)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_set_int64);
int MOCK_FUNC_WRAP(json_object_set_int64)(struct json_object *obj, int64_t new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_int64)) {
        check_expected_ptr(obj);
        check_expected(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_int64)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_set_uint64);
int MOCK_FUNC_WRAP(json_object_set_uint64)(struct json_object *obj, uint64_t new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_uint64)) {
        check_expected_ptr(obj);
        check_expected(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_uint64)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_new_double);
struct json_object *MOCK_FUNC_WRAP(json_object_new_double)(double d) {
    if (MOCK_IS_ACTIVE(json_object_new_double)) {
        check_expected(d);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_double)(d);
}

MOCK_FUNC_VAR_NEW(json_object_new_double_s);
struct json_object *MOCK_FUNC_WRAP(json_object_new_double_s)(double d, const char *ds) {
    if (MOCK_IS_ACTIVE(json_object_new_double_s)) {
        check_expected(d);
        check_expected_ptr(ds);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_double_s)(d, ds);
}

MOCK_FUNC_VAR_NEW(json_c_set_serialization_double_format);
int MOCK_FUNC_WRAP(json_c_set_serialization_double_format)(const char *double_format, int global_or_thread) {
    if (MOCK_IS_ACTIVE(json_c_set_serialization_double_format)) {
        check_expected_ptr(double_format);
        check_expected(global_or_thread);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_c_set_serialization_double_format)(double_format, global_or_thread);
}

MOCK_FUNC_VAR_NEW(json_object_double_to_json_string);
int MOCK_FUNC_WRAP(json_object_double_to_json_string)(struct json_object *jso, struct printbuf *pb, int level,
                                                      int flags) {
    if (MOCK_IS_ACTIVE(json_object_double_to_json_string)) {
        check_expected_ptr(jso);
        check_expected_ptr(pb);
        check_expected(level);
        check_expected(flags);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_double_to_json_string)(jso, pb, level, flags);
}

MOCK_FUNC_VAR_NEW(json_object_get_double);
double MOCK_FUNC_WRAP(json_object_get_double)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_double)) {
        check_expected_ptr(obj);
        return mock_type(double);
    }
    return MOCK_FUNC_REAL(json_object_get_double)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_set_double);
int MOCK_FUNC_WRAP(json_object_set_double)(struct json_object *obj, double new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_double)) {
        check_expected_ptr(obj);
        check_expected(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_double)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_new_string);
struct json_object *MOCK_FUNC_WRAP(json_object_new_string)(const char *s) {
    if (MOCK_IS_ACTIVE(json_object_new_string)) {
        check_expected_ptr(s);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_string)(s);
}

MOCK_FUNC_VAR_NEW(json_object_new_string_len);
struct json_object *MOCK_FUNC_WRAP(json_object_new_string_len)(const char *s, const int len) {
    if (MOCK_IS_ACTIVE(json_object_new_string_len)) {
        check_expected_ptr(s);
        check_expected(len);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_string_len)(s, len);
}

MOCK_FUNC_VAR_NEW(json_object_get_string);
int MOCK_FUNC_WRAP(json_object_get_string_errno = 0);
const char *MOCK_FUNC_WRAP(json_object_get_string)(struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_string)) {
        check_expected_ptr(obj);
        if (MOCK_FUNC_WRAP(json_object_get_string_errno) != 0) {
            errno = MOCK_FUNC_WRAP(json_object_get_string_errno);
            MOCK_FUNC_WRAP(json_object_get_string_errno) = 0;
        }
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(json_object_get_string)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_get_string_len);
int MOCK_FUNC_WRAP(json_object_get_string_len)(const struct json_object *obj) {
    if (MOCK_IS_ACTIVE(json_object_get_string_len)) {
        check_expected_ptr(obj);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_get_string_len)(obj);
}

MOCK_FUNC_VAR_NEW(json_object_set_string);
int MOCK_FUNC_WRAP(json_object_set_string)(json_object *obj, const char *new_value) {
    if (MOCK_IS_ACTIVE(json_object_set_string)) {
        check_expected_ptr(obj);
        check_expected_ptr(new_value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_string)(obj, new_value);
}

MOCK_FUNC_VAR_NEW(json_object_set_string_len);
int MOCK_FUNC_WRAP(json_object_set_string_len)(json_object *obj, const char *new_value, int len) {
    if (MOCK_IS_ACTIVE(json_object_set_string_len)) {
        check_expected_ptr(obj);
        check_expected_ptr(new_value);
        check_expected(len);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_set_string_len)(obj, new_value, len);
}

MOCK_FUNC_VAR_NEW(json_object_new_null);
struct json_object *MOCK_FUNC_WRAP(json_object_new_null)() {
    if (MOCK_IS_ACTIVE(json_object_new_null)) {
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_new_null)();
}

MOCK_FUNC_VAR_NEW(json_object_equal);
int MOCK_FUNC_WRAP(json_object_equal)(struct json_object *obj1, struct json_object *obj2) {
    if (MOCK_IS_ACTIVE(json_object_equal)) {
        check_expected_ptr(obj1);
        check_expected_ptr(obj2);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_equal)(obj1, obj2);
}

MOCK_FUNC_VAR_NEW(json_object_deep_copy);
int MOCK_FUNC_WRAP(json_object_deep_copy)(struct json_object *src, struct json_object **dst,
                                          json_c_shallow_copy_fn *shallow_copy) {
    if (MOCK_IS_ACTIVE(json_object_deep_copy)) {
        check_expected_ptr(src);
        check_expected_ptr(dst);
        check_expected_ptr(shallow_copy);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(json_object_deep_copy)(src, dst, shallow_copy);
}

MOCK_FUNC_VAR_NEW(json_tokener_parse);
struct json_object *MOCK_FUNC_WRAP(json_tokener_parse)(const char *str) {
    if (MOCK_IS_ACTIVE(json_tokener_parse)) {
        check_expected_ptr(str);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_tokener_parse)(str);
}

MOCK_FUNC_VAR_NEW(json_object_from_file);
struct json_object *MOCK_FUNC_WRAP(json_object_from_file)(const char *filename) {
    if (MOCK_IS_ACTIVE(json_object_from_file)) {
        check_expected_ptr(filename);
        return mock_ptr_type(json_object *);
    }
    return MOCK_FUNC_REAL(json_object_from_file)(filename);
}

MOCK_FUNC_VAR_NEW(json_util_get_last_err);
const char *MOCK_FUNC_WRAP(json_util_get_last_err)() {
    if (MOCK_IS_ACTIVE(json_util_get_last_err)) {
        return mock_ptr_type(const char *);
    }
    return MOCK_FUNC_REAL(json_util_get_last_err)();
}
