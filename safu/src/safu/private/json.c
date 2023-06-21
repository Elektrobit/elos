// SPDX-License-Identifier: MIT
#include "safu/json.h"

#include <errno.h>
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

#include "safu/common.h"
#include "safu/log.h"

#define TIMESPEC_ELEMENTS 2
#define TV_SEC_IDX        0
#define TV_NSEC_IDX       1

int safuJsonAddObject(struct json_object *jobj, const char *name, struct json_object *jdata) {
    int retval;

    if (name && json_object_get_type(jobj) == json_type_object) {
        retval = json_object_object_add(jobj, name, jdata);
        if (retval < 0) {
            safuLogErr("json_object_object_add failed!");
        }
    } else if (!name && json_object_get_type(jobj) == json_type_array) {
        retval = json_object_array_add(jobj, jdata);
        if (retval < 0) {
            safuLogErr("json_object_array_add failed!");
        }
    } else {
        retval = -1;
        safuLogWarn("json object is called with wrong index type!");
    }

    return retval;
}

struct json_object *safuJsonAddNewInt(struct json_object *jobj, const char *name, int32_t val) {
    struct json_object *jdata;

    jdata = json_object_new_int(val);
    if (!jdata) {
        safuLogErr("json_object_new_int failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewInt64(struct json_object *jobj, const char *name, int64_t val) {
    struct json_object *jdata;

    jdata = json_object_new_int64(val);
    if (!jdata) {
        safuLogErr("json_object_new_int64 failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewUint64(struct json_object *jobj, const char *name, uint64_t val) {
    struct json_object *jdata;

    jdata = json_object_new_uint64(val);
    if (!jdata) {
        safuLogErr("json_object_new_uint64 failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewString(struct json_object *jobj, const char *name, const char *val) {
    struct json_object *jdata;

    if (!val) {
        return NULL;
    }

    jdata = json_object_new_string(val);
    if (!jdata) {
        safuLogErr("json_object_new_string failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewArray(struct json_object *jobj, const char *name) {
    struct json_object *jdata;

    jdata = json_object_new_array();
    if (!jdata) {
        safuLogErr("json_object_new_array failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewArrayOfInitialSize(struct json_object *jobj, const char *name, int initialSize) {
    struct json_object *jdata;

    jdata = json_object_new_array_ext(initialSize);
    if (!jdata) {
        safuLogErr("json_object_new_array_ext failed!");
    } else if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        jdata = NULL;
    }

    return jdata;
}

struct json_object *safuJsonAddNewObject(struct json_object *jobj, const char *name) {
    struct json_object *jdata;

    jdata = json_object_new_object();
    if (!jdata) {
        safuLogErr("json_object_new_object failed!");
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jdata) < 0) {
        json_object_put(jdata);
        return NULL;
    }

    return jdata;
}

struct json_object *safuJsonGetObject(const struct json_object *jobj, const char *name, size_t idx) {
    const char *func;
    struct json_object *jdata;

    if (name && json_object_get_type(jobj) == json_type_object) {
        jdata = json_object_object_get(jobj, name);
        func = "json_object_object_get";
    } else if (!name && json_object_get_type(jobj) == json_type_array) {
        jdata = json_object_array_get_idx(jobj, idx);
        func = "json_object_array_get_idx";
    } else {
        safuLogWarn("failed, wrong index type!");
        return NULL;
    }

    if (jdata == NULL) {
        safuLogErrF("%s failed!", func);
    }

    return jdata;
}

struct json_object *safuJsonGetArray(const struct json_object *jobj, const char *name, size_t idx, size_t *len) {
    struct json_object *jdata;

    jdata = safuJsonGetObject(jobj, name, idx);
    if (!jdata || json_object_get_type(jdata) != json_type_array) {
        return NULL;
    }

    if (len != NULL) {
        *len = json_object_array_length(jdata);
    }

    return jdata;
}

int safuJsonGetInt32(const struct json_object *jobj, const char *name, size_t idx, int32_t *val) {
    struct json_object *jdata;

    jdata = safuJsonGetObject(jobj, name, idx);
    if (jdata == NULL) {
        return -1;
    }

    errno = 0;
    int32_t tmpVal = json_object_get_int(jdata);
    if (errno != 0) {
        safuLogErr("json_object_get_int failed!");
        return -1;
    }
    *val = tmpVal;

    return 0;
}

int safuJsonGetInt64(const struct json_object *jobj, const char *name, size_t idx, int64_t *val) {
    struct json_object *jdata;

    jdata = safuJsonGetObject(jobj, name, idx);
    if (jdata == NULL) {
        return -1;
    }

    errno = 0;
    int64_t tmpVal = json_object_get_int64(jdata);
    if (errno != 0) {
        safuLogErr("json_object_get_int64 failed!");
        return -1;
    }
    *val = tmpVal;

    return 0;
}

int safuJsonGetUint64(const struct json_object *jobj, const char *name, size_t idx, uint64_t *val) {
    struct json_object *jdata;

    jdata = safuJsonGetObject(jobj, name, idx);
    if (jdata == NULL) {
        return -1;
    }

    errno = 0;
    uint64_t tmpVal = json_object_get_uint64(jdata);
    if (errno != 0) {
        safuLogErr("json_object_get_uint64 failed!");
        return -1;
    }
    *val = tmpVal;

    return 0;
}

int safuJsonGetUint32(const struct json_object *jobj, const char *name, size_t idx, uint32_t *val) {
    uint64_t bigval;
    int retval;

    retval = safuJsonGetUint64(jobj, name, idx, &bigval);
    if (retval < 0) {
        return retval;
    }

    /*
     * json_object_get_uint64 clamps out of range numbers to the uint64_t
     * range, so do the same for uint32_t.
     */
    if (bigval > UINT32_MAX) {
        *val = UINT32_MAX;
    } else {
        *val = bigval;
    }
    return 0;
}

int safuJsonGetString(const struct json_object *jobj, const char *name, size_t idx, const char **val) {
    struct json_object *jdata;

    jdata = safuJsonGetObject(jobj, name, idx);
    if (jdata == NULL) {
        return -1;
    }

    errno = 0;
    const char *tmpVal = json_object_get_string(jdata);
    if (errno != 0) {
        safuLogErr("json_object_get_string failed!");
        return -1;
    }
    *val = tmpVal;

    return 0;
}

struct json_object *safuJsonAddNewTimestamp(struct json_object *jobj, const char *name,
                                            const struct timespec *timestamp) {
    struct json_object *jarray;
    struct json_object *jTime;

    if (!timestamp) {
        safuLogErr("no timestamp providet!");
        return NULL;
    }

    jarray = json_object_new_array();
    if (!jarray) {
        safuLogErr("json_object_new_array failed!");
        return NULL;
    }

    jTime = json_object_new_int64(timestamp->tv_sec);
    if (!jTime) {
        safuLogErr("json_object_new_int64 failed!");
        json_object_put(jarray);
        return NULL;
    }

    if (json_object_array_add(jarray, jTime) < 0) {
        safuLogErr("json_object_array_add failed!");
        json_object_put(jTime);
        json_object_put(jarray);
        return NULL;
    }

    jTime = json_object_new_int64(timestamp->tv_nsec);
    if (!jTime) {
        safuLogErr("json_object_new_int64 failed!");
        json_object_put(jarray);
        return NULL;
    }

    if (json_object_array_add(jarray, jTime) < 0) {
        safuLogErr("json_object_array_add failed!");
        json_object_put(jTime);
        json_object_put(jarray);
        return NULL;
    }

    if (safuJsonAddObject(jobj, name, jarray) < 0) {
        safuLogErr("safuJsonAddObject failed!");
        json_object_put(jarray);
        return NULL;
    }

    return jarray;
}

int safuJsonGetTimestamp(const struct json_object *jobj, const char *name, size_t idx, struct timespec *timestamp) {
    struct json_object *jtimestamp;
    size_t arrayLength = 0;
    int64_t tmpSecs;

    jtimestamp = safuJsonGetArray(jobj, name, idx, &arrayLength);
    if (!jtimestamp) {
        safuLogErr("safuJsonGetArray failed");
        return -1;
    }

    if (arrayLength == 0 || arrayLength != TIMESPEC_ELEMENTS) {
        safuLogErr("Invalid array length");
        return -1;
    }

    if (safuJsonGetInt64(jtimestamp, NULL, TV_SEC_IDX, &tmpSecs)) {
        safuLogErr("safuJsonGetInt64 (TV_SEC_IDX) failed");
        return -1;
    }

    if (safuJsonGetInt64(jtimestamp, NULL, TV_NSEC_IDX, &timestamp->tv_nsec)) {
        safuLogErr("safuJsonGetInt64 (TV_NSEC_IDX) failed");
        return -1;
    }
    timestamp->tv_sec = tmpSecs;

    return 0;
}
