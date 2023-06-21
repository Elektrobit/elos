// SPDX-License-Identifier: MIT

#ifndef __MOCK_JSONC_H__
#define __MOCK_JSONC_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_extensions/mock_func_wrap.h>
#include <errno.h>
#include <json-c/arraylist.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/linkhash.h>
#include <stdbool.h>

// clang-format off
// because this order is a cmocka requirement
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <cmocka.h>
// clang-format on

extern int MOCK_FUNC_WRAP(json_object_get_int_errno);
extern int MOCK_FUNC_WRAP(json_object_get_string_errno);
extern int MOCK_FUNC_WRAP(json_object_get_uint64_errno);
extern int MOCK_FUNC_WRAP(json_object_get_int64_errno);

MOCK_FUNC_VAR_EXTERN(json_object_new_object);
struct json_object *MOCK_FUNC_WRAP(json_object_new_object)();
struct json_object *MOCK_FUNC_REAL(json_object_new_object)();

MOCK_FUNC_VAR_EXTERN(json_object_get);
struct json_object *MOCK_FUNC_WRAP(json_object_get)(struct json_object *obj);
struct json_object *MOCK_FUNC_REAL(json_object_get)(struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_put);
int MOCK_FUNC_WRAP(json_object_put)(struct json_object *obj);
int MOCK_FUNC_REAL(json_object_put)(struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_is_type);
int MOCK_FUNC_WRAP(json_object_is_type)(const struct json_object *obj, enum json_type type);
int MOCK_FUNC_REAL(json_object_is_type)(const struct json_object *obj, enum json_type type);

MOCK_FUNC_VAR_EXTERN(json_object_get_type);
enum json_type MOCK_FUNC_WRAP(json_object_get_type)(const struct json_object *obj);
enum json_type MOCK_FUNC_REAL(json_object_get_type)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_to_json_string);
const char *MOCK_FUNC_WRAP(json_object_to_json_string)(struct json_object *obj);
const char *MOCK_FUNC_REAL(json_object_to_json_string)(struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_to_json_string_ext);
const char *MOCK_FUNC_WRAP(json_object_to_json_string_ext)(struct json_object *obj, int flags);
const char *MOCK_FUNC_REAL(json_object_to_json_string_ext)(struct json_object *obj, int flags);

MOCK_FUNC_VAR_EXTERN(json_object_to_json_string_length);
const char *MOCK_FUNC_WRAP(json_object_to_json_string_length)(struct json_object *obj, int flags, size_t *length);
const char *MOCK_FUNC_REAL(json_object_to_json_string_length)(struct json_object *obj, int flags, size_t *length);

MOCK_FUNC_VAR_EXTERN(json_object_get_userdata);
void *MOCK_FUNC_WRAP(json_object_get_userdata)(json_object *jso);
void *MOCK_FUNC_REAL(json_object_get_userdata)(json_object *jso);

MOCK_FUNC_VAR_EXTERN(json_object_set_userdata);
void MOCK_FUNC_WRAP(json_object_set_userdata)(json_object *jso, void *userdata, json_object_delete_fn *user_delete);
void MOCK_FUNC_REAL(json_object_set_userdata)(json_object *jso, void *userdata, json_object_delete_fn *user_delete);

MOCK_FUNC_VAR_EXTERN(json_object_set_serializer);
void MOCK_FUNC_WRAP(json_object_set_serializer)(json_object *jso, json_object_to_json_string_fn *to_string_func,
                                                void *userdata, json_object_delete_fn *user_delete);
void MOCK_FUNC_REAL(json_object_set_serializer)(json_object *jso, json_object_to_json_string_fn *to_string_func,
                                                void *userdata, json_object_delete_fn *user_delete);

MOCK_FUNC_VAR_EXTERN(json_object_get_object);
struct lh_table *MOCK_FUNC_WRAP(json_object_get_object)(const struct json_object *obj);
struct lh_table *MOCK_FUNC_REAL(json_object_get_object)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_object_length);
int MOCK_FUNC_WRAP(json_object_object_length)(const struct json_object *obj);
int MOCK_FUNC_REAL(json_object_object_length)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_object_add);
int MOCK_FUNC_WRAP(json_object_object_add)(struct json_object *obj, const char *key, struct json_object *val);
int MOCK_FUNC_REAL(json_object_object_add)(struct json_object *obj, const char *key, struct json_object *val);

MOCK_FUNC_VAR_EXTERN(json_object_object_add_ex);
int MOCK_FUNC_WRAP(json_object_object_add_ex)(struct json_object *obj, const char *const key,
                                              struct json_object *const val, const unsigned opts);
int MOCK_FUNC_REAL(json_object_object_add_ex)(struct json_object *obj, const char *const key,
                                              struct json_object *const val, const unsigned opts);

MOCK_FUNC_VAR_EXTERN(json_object_object_get);
struct json_object *MOCK_FUNC_WRAP(json_object_object_get)(const struct json_object *obj, const char *key);
struct json_object *MOCK_FUNC_REAL(json_object_object_get)(const struct json_object *obj, const char *key);

MOCK_FUNC_VAR_EXTERN(json_object_object_get_ex);
json_bool MOCK_FUNC_WRAP(json_object_object_get_ex)(const struct json_object *obj, const char *key,
                                                    struct json_object **value);
json_bool MOCK_FUNC_REAL(json_object_object_get_ex)(const struct json_object *obj, const char *key,
                                                    struct json_object **value);

MOCK_FUNC_VAR_EXTERN(json_object_object_del);
void MOCK_FUNC_WRAP(json_object_object_del)(struct json_object *obj, const char *key);
void MOCK_FUNC_REAL(json_object_object_del)(struct json_object *obj, const char *key);

MOCK_FUNC_VAR_EXTERN(json_object_new_array);
struct json_object *MOCK_FUNC_WRAP(json_object_new_array)();
struct json_object *MOCK_FUNC_REAL(json_object_new_array)();

MOCK_FUNC_VAR_EXTERN(json_object_new_array_ext);
struct json_object *MOCK_FUNC_WRAP(json_object_new_array_ext)(int initial_size);
struct json_object *MOCK_FUNC_REAL(json_object_new_array_ext)(int initial_size);

MOCK_FUNC_VAR_EXTERN(json_object_get_array);
struct array_list *MOCK_FUNC_WRAP(json_object_get_array)(const struct json_object *obj);
struct array_list *MOCK_FUNC_REAL(json_object_get_array)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_array_length);
size_t MOCK_FUNC_WRAP(json_object_array_length)(const struct json_object *obj);
size_t MOCK_FUNC_REAL(json_object_array_length)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_array_add);
int MOCK_FUNC_WRAP(json_object_array_add)(struct json_object *obj, struct json_object *val);
int MOCK_FUNC_REAL(json_object_array_add)(struct json_object *obj, struct json_object *val);

MOCK_FUNC_VAR_EXTERN(json_object_array_put_idx);
int MOCK_FUNC_WRAP(json_object_array_put_idx)(struct json_object *obj, size_t idx, struct json_object *val);
int MOCK_FUNC_REAL(json_object_array_put_idx)(struct json_object *obj, size_t idx, struct json_object *val);

MOCK_FUNC_VAR_EXTERN(json_object_array_get_idx);
struct json_object *MOCK_FUNC_WRAP(json_object_array_get_idx)(const struct json_object *obj, size_t idx);
struct json_object *MOCK_FUNC_REAL(json_object_array_get_idx)(const struct json_object *obj, size_t idx);

MOCK_FUNC_VAR_EXTERN(json_object_array_del_idx);
int MOCK_FUNC_WRAP(json_object_array_del_idx)(struct json_object *obj, size_t idx, size_t count);
int MOCK_FUNC_REAL(json_object_array_del_idx)(struct json_object *obj, size_t idx, size_t count);

MOCK_FUNC_VAR_EXTERN(json_object_array_shrink);
int MOCK_FUNC_WRAP(json_object_array_shrink)(struct json_object *jso, int empty_slots);
int MOCK_FUNC_REAL(json_object_array_shrink)(struct json_object *jso, int empty_slots);

MOCK_FUNC_VAR_EXTERN(json_object_new_boolean);
struct json_object *MOCK_FUNC_WRAP(json_object_new_boolean)(json_bool b);
struct json_object *MOCK_FUNC_REAL(json_object_new_boolean)(json_bool b);

MOCK_FUNC_VAR_EXTERN(json_object_get_boolean);
json_bool MOCK_FUNC_WRAP(json_object_get_boolean)(const struct json_object *obj);
json_bool MOCK_FUNC_REAL(json_object_get_boolean)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_set_boolean);
int MOCK_FUNC_WRAP(json_object_set_boolean)(struct json_object *obj, json_bool new_value);
int MOCK_FUNC_REAL(json_object_set_boolean)(struct json_object *obj, json_bool new_value);

MOCK_FUNC_VAR_EXTERN(json_object_new_int);
struct json_object *MOCK_FUNC_WRAP(json_object_new_int)(int32_t i);
struct json_object *MOCK_FUNC_REAL(json_object_new_int)(int32_t i);

MOCK_FUNC_VAR_EXTERN(json_object_new_int64);
struct json_object *MOCK_FUNC_WRAP(json_object_new_int64)(int64_t i);
struct json_object *MOCK_FUNC_REAL(json_object_new_int64)(int64_t i);

MOCK_FUNC_VAR_EXTERN(json_object_new_uint64);
struct json_object *MOCK_FUNC_WRAP(json_object_new_uint64)(uint64_t i);
struct json_object *MOCK_FUNC_REAL(json_object_new_uint64)(uint64_t i);

MOCK_FUNC_VAR_EXTERN(json_object_get_int);
int32_t MOCK_FUNC_WRAP(json_object_get_int)(const struct json_object *obj);
int32_t MOCK_FUNC_REAL(json_object_get_int)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_set_int);
int MOCK_FUNC_WRAP(json_object_set_int)(struct json_object *obj, int new_value);
int MOCK_FUNC_REAL(json_object_set_int)(struct json_object *obj, int new_value);

MOCK_FUNC_VAR_EXTERN(json_object_int_inc);
int MOCK_FUNC_WRAP(json_object_int_inc)(struct json_object *obj, int64_t val);
int MOCK_FUNC_REAL(json_object_int_inc)(struct json_object *obj, int64_t val);

MOCK_FUNC_VAR_EXTERN(json_object_get_int64);
int64_t MOCK_FUNC_WRAP(json_object_get_int64)(const struct json_object *obj);
int64_t MOCK_FUNC_REAL(json_object_get_int64)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_get_uint64);
uint64_t MOCK_FUNC_WRAP(json_object_get_uint64)(const struct json_object *obj);
uint64_t MOCK_FUNC_REAL(json_object_get_uint64)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_set_int64);
int MOCK_FUNC_WRAP(json_object_set_int64)(struct json_object *obj, int64_t new_value);
int MOCK_FUNC_REAL(json_object_set_int64)(struct json_object *obj, int64_t new_value);

MOCK_FUNC_VAR_EXTERN(json_object_set_uint64);
int MOCK_FUNC_WRAP(json_object_set_uint64)(struct json_object *obj, uint64_t new_value);
int MOCK_FUNC_REAL(json_object_set_uint64)(struct json_object *obj, uint64_t new_value);

MOCK_FUNC_VAR_EXTERN(json_object_new_double);
struct json_object *MOCK_FUNC_WRAP(json_object_new_double)(double d);
struct json_object *MOCK_FUNC_REAL(json_object_new_double)(double d);

MOCK_FUNC_VAR_EXTERN(json_object_new_double_s);
struct json_object *MOCK_FUNC_WRAP(json_object_new_double_s)(double d, const char *ds);
struct json_object *MOCK_FUNC_REAL(json_object_new_double_s)(double d, const char *ds);

MOCK_FUNC_VAR_EXTERN(json_c_set_serialization_double_format);
int MOCK_FUNC_WRAP(json_c_set_serialization_double_format)(const char *double_format, int global_or_thread);
int MOCK_FUNC_REAL(json_c_set_serialization_double_format)(const char *double_format, int global_or_thread);

MOCK_FUNC_VAR_EXTERN(json_object_double_to_json_string);
int MOCK_FUNC_WRAP(json_object_double_to_json_string)(struct json_object *jso, struct printbuf *pb, int level,
                                                      int flags);
int MOCK_FUNC_REAL(json_object_double_to_json_string)(struct json_object *jso, struct printbuf *pb, int level,
                                                      int flags);

MOCK_FUNC_VAR_EXTERN(json_object_get_double);
double MOCK_FUNC_WRAP(json_object_get_double)(const struct json_object *obj);
double MOCK_FUNC_REAL(json_object_get_double)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_set_double);
int MOCK_FUNC_WRAP(json_object_set_double)(struct json_object *obj, double new_value);
int MOCK_FUNC_REAL(json_object_set_double)(struct json_object *obj, double new_value);

MOCK_FUNC_VAR_EXTERN(json_object_new_string);
struct json_object *MOCK_FUNC_WRAP(json_object_new_string)(const char *s);
struct json_object *MOCK_FUNC_REAL(json_object_new_string)(const char *s);

MOCK_FUNC_VAR_EXTERN(json_object_new_string_len);
struct json_object *MOCK_FUNC_WRAP(json_object_new_string_len)(const char *s, const int len);
struct json_object *MOCK_FUNC_REAL(json_object_new_string_len)(const char *s, const int len);

MOCK_FUNC_VAR_EXTERN(json_object_get_string);
const char *MOCK_FUNC_WRAP(json_object_get_string)(struct json_object *obj);
const char *MOCK_FUNC_REAL(json_object_get_string)(struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_get_string_len);
int MOCK_FUNC_WRAP(json_object_get_string_len)(const struct json_object *obj);
int MOCK_FUNC_REAL(json_object_get_string_len)(const struct json_object *obj);

MOCK_FUNC_VAR_EXTERN(json_object_set_string);
int MOCK_FUNC_WRAP(json_object_set_string)(json_object *obj, const char *new_value);
int MOCK_FUNC_REAL(json_object_set_string)(json_object *obj, const char *new_value);

MOCK_FUNC_VAR_EXTERN(json_object_set_string_len);
int MOCK_FUNC_WRAP(json_object_set_string_len)(json_object *obj, const char *new_value, int len);
int MOCK_FUNC_REAL(json_object_set_string_len)(json_object *obj, const char *new_value, int len);

MOCK_FUNC_VAR_EXTERN(json_object_new_null);
struct json_object *MOCK_FUNC_WRAP(json_object_new_null)();
struct json_object *MOCK_FUNC_REAL(json_object_new_null)();

MOCK_FUNC_VAR_EXTERN(json_object_equal);
int MOCK_FUNC_WRAP(json_object_equal)(struct json_object *obj1, struct json_object *obj2);
int MOCK_FUNC_REAL(json_object_equal)(struct json_object *obj1, struct json_object *obj2);

MOCK_FUNC_VAR_EXTERN(json_object_deep_copy);
int MOCK_FUNC_WRAP(json_object_deep_copy)(struct json_object *src, struct json_object **dst,
                                          json_c_shallow_copy_fn *shallow_copy);
int MOCK_FUNC_REAL(json_object_deep_copy)(struct json_object *src, struct json_object **dst,
                                          json_c_shallow_copy_fn *shallow_copy);

MOCK_FUNC_VAR_EXTERN(json_tokener_parse);
struct json_object *MOCK_FUNC_WRAP(json_tokener_parse)(const char *str);
struct json_object *MOCK_FUNC_REAL(json_tokener_parse)(const char *str);

MOCK_FUNC_VAR_EXTERN(json_object_from_file);
struct json_object *MOCK_FUNC_WRAP(json_object_from_file)(const char *_path);
struct json_object *MOCK_FUNC_REAL(json_object_from_file)(const char *_path);

MOCK_FUNC_VAR_EXTERN(json_util_get_last_err);
const char *MOCK_FUNC_WRAP(json_util_get_last_err)();
const char *MOCK_FUNC_REAL(json_util_get_last_err)();

#endif /* __MOCK_JSONC_H__ */
