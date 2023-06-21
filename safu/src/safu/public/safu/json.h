// SPDX-License-Identifier: MIT
#ifndef SAFU_JSON_H
#define SAFU_JSON_H

#include <json-c/json_types.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

struct json_object *safuJsonAddNewArray(struct json_object *jobj, const char *name);
struct json_object *safuJsonAddNewArrayOfInitialSize(struct json_object *jobj, const char *name, int initialSize);
struct json_object *safuJsonAddNewInt(struct json_object *jobj, const char *name, int32_t val);
struct json_object *safuJsonAddNewInt64(struct json_object *jobj, const char *name, int64_t val);
struct json_object *safuJsonAddNewObject(struct json_object *jobj, const char *name);
struct json_object *safuJsonAddNewString(struct json_object *jobj, const char *name, const char *val);
struct json_object *safuJsonAddNewUint64(struct json_object *jobj, const char *name, uint64_t val);
int safuJsonAddObject(struct json_object *jobj, const char *name, struct json_object *jdata);
int safuJsonGetInt32(const struct json_object *jobj, const char *name, size_t idx, int32_t *val);
int safuJsonGetInt64(const struct json_object *jobj, const char *name, size_t idx, int64_t *val);
struct json_object *safuJsonGetObject(const struct json_object *jobj, const char *name, size_t idx);
struct json_object *safuJsonGetArray(const struct json_object *jobj, const char *name, size_t idx, size_t *len);
int safuJsonGetUint32(const struct json_object *jobj, const char *name, size_t idx, uint32_t *val);
int safuJsonGetUint64(const struct json_object *jobj, const char *name, size_t idx, uint64_t *val);
int safuJsonGetString(const struct json_object *jobj, const char *name, size_t idx, const char **val);
struct json_object *safuJsonAddNewTimestamp(struct json_object *jobj, const char *name,
                                            const struct timespec *timestamp);
int safuJsonGetTimestamp(const struct json_object *jobj, const char *name, size_t idx, struct timespec *timestamp);

#endif
