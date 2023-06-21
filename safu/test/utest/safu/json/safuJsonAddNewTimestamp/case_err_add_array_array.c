// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

int safuTestSafuJsonAddNewTimestampErrAddArrayArraySetup(UNUSED void **state) {
    return 0;
}
int safuTestSafuJsonAddNewTimestampErrAddArrayArrayTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(struct timespec timestamp) {
    PARAM("timestamp = { tv_sec = %ld, tv_nsec = %ld }", timestamp.tv_sec, timestamp.tv_nsec);

    MOCK_FUNC_ALWAYS(json_object_new_array);
    MOCK_FUNC_ALWAYS(json_object_new_int64);
    MOCK_FUNC_ALWAYS(json_object_array_add);

    struct json_object *jtimewrap = __real_json_object_new_array();
    will_return(__wrap_json_object_new_array, jtimewrap);

    struct json_object *jtvSec = __real_json_object_new_int64(timestamp.tv_sec);
    expect_value(__wrap_json_object_new_int64, i, timestamp.tv_sec);
    will_return(__wrap_json_object_new_int64, jtvSec);

    struct json_object *jtvNsec = __real_json_object_new_int64(timestamp.tv_nsec);
    expect_value(__wrap_json_object_new_int64, i, timestamp.tv_nsec);
    will_return(__wrap_json_object_new_int64, jtvNsec);

    expect_value(__wrap_json_object_array_add, obj, jtimewrap);
    expect_value(__wrap_json_object_array_add, val, jtvSec);
    will_return(__wrap_json_object_array_add, __real_json_object_array_add(jtimewrap, jtvSec));

    expect_value(__wrap_json_object_array_add, obj, jtimewrap);
    expect_value(__wrap_json_object_array_add, val, jtvNsec);
    will_return(__wrap_json_object_array_add, __real_json_object_array_add(jtimewrap, jtvNsec));

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(NULL, NULL, &timestamp);

    assert_null(jtimestamp);

    MOCK_FUNC_NEVER(json_object_new_array);
    MOCK_FUNC_NEVER(json_object_new_int64);
    MOCK_FUNC_NEVER(json_object_array_add);
}

void safuTestSafuJsonAddNewTimestampErrAddArrayArray(UNUSED void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to add an array for a timestamp to a json array");

    const struct timespec testRows[] = {
        {0, 0},         {0, 42},         {0, INT64_MAX},         {0, INT64_MIN},         {0, -12},
        {42, 0},        {42, 42},        {42, INT64_MAX},        {42, INT64_MIN},        {42, -12},
        {INT64_MAX, 0}, {INT64_MAX, 42}, {INT64_MAX, INT64_MAX}, {INT64_MAX, INT64_MIN}, {INT64_MAX, -12},
        {INT64_MIN, 0}, {INT64_MIN, 42}, {INT64_MIN, INT64_MAX}, {INT64_MIN, INT64_MIN}, {INT64_MIN, -12},
        {-12, 0},       {-12, 42},       {-12, INT64_MAX},       {-12, INT64_MIN},       {-12, -12},
    };
    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(testRows[i]);
    }
}
