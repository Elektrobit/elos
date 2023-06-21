// SPDX-License-Identifier: MIT

#ifndef __MOCK_SAFU_H__
#define __MOCK_SAFU_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_SAFU_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "safu/common.h"
#include "safu/json.h"

MOCK_FUNC_PROTOTYPE(safuGetEnvOr, const char *, const char *key, const char *defaultValue)
MOCK_FUNC_PROTOTYPE(safuReadFileToString, ssize_t, const char *fileName, ssize_t maxLength, char **string)
MOCK_FUNC_PROTOTYPE(safuWriteExactly, ssize_t, int fd, const void *buf, size_t len)
MOCK_FUNC_PROTOTYPE(safuRecvExactly, ssize_t, int fd, void *buf, size_t len)
MOCK_FUNC_PROTOTYPE(safuSendExactly, ssize_t, int fd, const void *buf, size_t len)
MOCK_FUNC_PROTOTYPE(safuAllocMem, void *, void *oldptr, size_t newlen)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewInt, struct json_object *, struct json_object *jobj, const char *name, int32_t val)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewString, struct json_object *, struct json_object *jobj, const char *name,
                    const char *val)
MOCK_FUNC_PROTOTYPE(safuJsonAddObject, int, struct json_object *jobj, const char *name, struct json_object *jdata)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewInt64, struct json_object *, struct json_object *jobj, const char *name, int64_t val)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewUint64, struct json_object *, struct json_object *jobj, const char *name,
                    uint64_t val)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewArray, struct json_object *, struct json_object *jobj, const char *name)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewArrayOfInitialSize, struct json_object *, struct json_object *jobj, const char *name,
                    int initialSize)
MOCK_FUNC_PROTOTYPE(safuJsonAddNewTimestamp, struct json_object *, struct json_object *jobj, const char *name,
                    const struct timespec *timestamp)
MOCK_FUNC_PROTOTYPE(safuJsonGetInt32, int, const struct json_object *jobj, const char *name, size_t idx, int32_t *val)
MOCK_FUNC_PROTOTYPE(safuJsonGetInt64, int, const struct json_object *jobj, const char *name, size_t idx, int64_t *val)
MOCK_FUNC_PROTOTYPE(safuJsonGetObject, struct json_object *, const struct json_object *jobj, const char *name,
                    size_t idx)
MOCK_FUNC_PROTOTYPE(safuJsonGetArray, struct json_object *, const struct json_object *jobj, const char *name,
                    size_t idx, size_t *len)
MOCK_FUNC_PROTOTYPE(safuJsonGetUint32, int, const struct json_object *jobj, const char *name, size_t idx, uint32_t *val)
MOCK_FUNC_PROTOTYPE(safuJsonGetUint64, int, const struct json_object *jobj, const char *name, size_t idx, uint64_t *val)
MOCK_FUNC_PROTOTYPE(safuJsonGetString, int, const struct json_object *jobj, const char *name, size_t idx,
                    const char **val)
MOCK_FUNC_PROTOTYPE(safuJsonGetTimestamp, int, const struct json_object *jobj, const char *name, size_t idx,
                    struct timespec *timestamp)
MOCK_FUNC_PROTOTYPE(safuTimeGetLocalTime, safuResultE_t, struct tm *localTime)

MOCK_FUNC_PROTOTYPE(safuGetHardwareId, const char *, void)
#endif /* __MOCK_SAFU_H__ */
