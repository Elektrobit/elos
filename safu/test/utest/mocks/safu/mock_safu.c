// SPDX-License-Identifier: MIT

#include "mock_safu.h"

#include <stdbool.h>
#include <stdlib.h>

MOCK_FUNC_BODY(safuGetEnvOr, const char *, const char *key, const char *defaultValue) {
    if (MOCK_IS_ACTIVE(safuGetEnvOr)) {
        check_expected_ptr(key);
        check_expected_ptr(defaultValue);
        return mock_ptr_type(const char *);
    }
    return MOCK_FUNC_REAL(safuGetEnvOr)(key, defaultValue);
}

MOCK_FUNC_BODY(safuReadFileToString, ssize_t, const char *fileName, ssize_t maxLength, char **string) {
    if (MOCK_IS_ACTIVE(safuReadFileToString)) {
        check_expected_ptr(fileName);
        check_expected(maxLength);
        check_expected_ptr(string);
        if (string != NULL) {
            *string = mock_ptr_type(char *);
        }
        return mock_type(ssize_t);
    }
    return MOCK_FUNC_REAL(safuReadFileToString)(fileName, maxLength, string);
}

MOCK_FUNC_BODY(safuWriteExactly, ssize_t, int fd, const void *buf, size_t len) {
    if (MOCK_IS_ACTIVE(safuWriteExactly)) {
        check_expected(fd);
        check_expected_ptr(buf);
        check_expected(len);
        return mock_type(ssize_t);
    }
    return MOCK_FUNC_REAL(safuWriteExactly)(fd, buf, len);
}

MOCK_FUNC_BODY(safuRecvExactly, safuResultE_t, int fd, void *buf, size_t len, size_t *transferred) {
    if (MOCK_IS_ACTIVE(safuRecvExactly)) {
        check_expected(fd);
        check_expected_ptr(buf);
        check_expected(len);
        if (buf != NULL) {
            memcpy(buf, mock_ptr_type(void *), len);
        }
        *transferred = mock_type(size_t);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(safuRecvExactly)(fd, buf, len, transferred);
}

MOCK_FUNC_BODY(safuSendExactly, safuResultE_t, int fd, const void *buf, size_t len, size_t *transferred) {
    if (MOCK_IS_ACTIVE(safuSendExactly)) {
        check_expected(fd);
        check_expected_ptr(buf);
        check_expected(len);
        *transferred = mock_type(size_t);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(safuSendExactly)(fd, buf, len, transferred);
}

MOCK_FUNC_BODY(safuAllocMem, void *, void *oldptr, size_t newlen) {
    if (MOCK_IS_ACTIVE(safuAllocMem)) {
        check_expected_ptr(oldptr);
        if (oldptr != NULL) free(oldptr);
        check_expected(newlen);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(safuAllocMem)(oldptr, newlen);
}

MOCK_FUNC_BODY(safuJsonAddObject, int, struct json_object *jobj, const char *name, struct json_object *jdata) {
    if (MOCK_IS_ACTIVE(safuJsonAddObject)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected_ptr(jdata);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonAddObject)(jobj, name, jdata);
}

MOCK_FUNC_BODY(safuJsonAddNewInt, struct json_object *, struct json_object *jobj, const char *name, int32_t val) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewInt)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(val);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewInt)(jobj, name, val);
}

MOCK_FUNC_BODY(safuJsonAddNewInt64, struct json_object *, struct json_object *jobj, const char *name, int64_t val) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewInt64)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(val);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewInt64)(jobj, name, val);
}

MOCK_FUNC_BODY(safuJsonAddNewUint64, struct json_object *, struct json_object *jobj, const char *name, uint64_t val) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewUint64)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(val);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewUint64)(jobj, name, val);
}

MOCK_FUNC_BODY(safuJsonAddNewString, struct json_object *, struct json_object *jobj, const char *name,
               const char *val) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewString)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected_ptr(val);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewString)(jobj, name, val);
}

MOCK_FUNC_BODY(safuJsonAddNewArray, struct json_object *, struct json_object *jobj, const char *name) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewArray)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewArray)(jobj, name);
}

MOCK_FUNC_BODY(safuJsonAddNewArrayOfInitialSize, struct json_object *, struct json_object *jobj, const char *name,
               int initialSize) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewArrayOfInitialSize)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(initialSize);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewArrayOfInitialSize)(jobj, name, initialSize);
}

MOCK_FUNC_BODY(safuJsonAddNewTimestamp, struct json_object *, struct json_object *jobj, const char *name,
               const struct timespec *timestamp) {
    if (MOCK_IS_ACTIVE(safuJsonAddNewTimestamp)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected_ptr(timestamp);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonAddNewTimestamp)(jobj, name, timestamp);
}

MOCK_FUNC_BODY(safuJsonGetInt32, int, const struct json_object *jobj, const char *name, size_t idx, int32_t *val) {
    if (MOCK_IS_ACTIVE(safuJsonGetInt32)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(val);
        if (val != NULL) {
            *val = mock_type(int32_t);
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetInt32)(jobj, name, idx, val);
}

MOCK_FUNC_BODY(safuJsonGetInt64, int, const struct json_object *jobj, const char *name, size_t idx, int64_t *val) {
    if (MOCK_IS_ACTIVE(safuJsonGetInt64)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(val);
        if (val != NULL) {
            *val = mock_type(int64_t);
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetInt64)(jobj, name, idx, val);
}

MOCK_FUNC_BODY(safuJsonGetObject, struct json_object *, const struct json_object *jobj, const char *name, size_t idx) {
    if (MOCK_IS_ACTIVE(safuJsonGetObject)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonGetObject)(jobj, name, idx);
}

MOCK_FUNC_BODY(safuJsonGetArray, struct json_object *, const struct json_object *jobj, const char *name, size_t idx,
               size_t *len) {
    if (MOCK_IS_ACTIVE(safuJsonGetArray)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected(len);
        if (len != NULL) {
            *len = mock_type(size_t);
        }
        return mock_ptr_type(struct json_object *);
    }
    return MOCK_FUNC_REAL(safuJsonGetArray)(jobj, name, idx, len);
}

MOCK_FUNC_BODY(safuJsonGetUint32, int, const struct json_object *jobj, const char *name, size_t idx, uint32_t *val) {
    if (MOCK_IS_ACTIVE(safuJsonGetUint32)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(val);
        if (val != NULL) {
            *val = mock_type(uint32_t);
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetUint32)(jobj, name, idx, val);
}

MOCK_FUNC_BODY(safuJsonGetUint64, int, const struct json_object *jobj, const char *name, size_t idx, uint64_t *val) {
    if (MOCK_IS_ACTIVE(safuJsonGetUint64)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(val);
        if (val != NULL) {
            *val = mock_type(uint64_t);
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetUint64)(jobj, name, idx, val);
}

MOCK_FUNC_BODY(safuJsonGetString, int, const struct json_object *jobj, const char *name, size_t idx, const char **val) {
    if (MOCK_IS_ACTIVE(safuJsonGetString)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(val);
        if (val != NULL) {
            *val = mock_type(const char *);
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetString)(jobj, name, idx, val);
}

MOCK_FUNC_BODY(safuJsonGetTimestamp, int, const struct json_object *jobj, const char *name, size_t idx,
               struct timespec *timestamp) {
    if (MOCK_IS_ACTIVE(safuJsonGetTimestamp)) {
        check_expected_ptr(jobj);
        check_expected_ptr(name);
        check_expected(idx);
        check_expected_ptr(timestamp);
        timestamp->tv_sec = mock_type(int);
        timestamp->tv_nsec = mock_type(int);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuJsonGetTimestamp)(jobj, name, idx, timestamp);
}

MOCK_FUNC_BODY(safuTimeGetLocalTime, safuResultE_t, struct tm *localTime) {
    if (MOCK_IS_ACTIVE(safuTimeGetLocalTime)) {
        check_expected_ptr(localTime);
        localTime->tm_sec = mock_type(int);
        localTime->tm_min = mock_type(int);
        localTime->tm_hour = mock_type(int);
        localTime->tm_mday = mock_type(int);
        localTime->tm_mon = mock_type(int);
        localTime->tm_year = mock_type(int);
        localTime->tm_wday = mock_type(int);
        localTime->tm_yday = mock_type(int);
        localTime->tm_isdst = mock_type(int);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(safuTimeGetLocalTime)(localTime);
}

MOCK_FUNC_BODY(safuGetHardwareId, const char *, void) {
    if (MOCK_IS_ACTIVE(safuGetHardwareId)) {
        return mock_type(const char *);
    }
    return MOCK_FUNC_REAL(safuGetHardwareId)();
}
