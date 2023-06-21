// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

SETUP(safuTestSafuJsonAddNewTimestampErrInsertNSecArray)
TEARDOWN(safuTestSafuJsonAddNewTimestampErrInsertNSecArray)

static void _testCase(struct json_object *jobj, size_t idx, struct timespec timestamp) {
    PARAM("idx = %lu", idx);
    PARAM("timestamp = { tv_sec = %ld, tv_nsec = %ld }", timestamp.tv_sec, timestamp.tv_nsec);

    MOCK_FUNC_ALWAYS(json_object_new_array);
    MOCK_FUNC_ALWAYS(json_object_new_int64);
    MOCK_FUNC_ALWAYS(json_object_array_add);

    struct json_object *jtimewrap = __real_json_object_new_array();
    will_return(__wrap_json_object_new_array, jtimewrap);

    struct json_object *jtvSec = __real_json_object_new_int64(timestamp.tv_sec);
    expect_value(__wrap_json_object_new_int64, i, timestamp.tv_sec);
    will_return(__wrap_json_object_new_int64, jtvSec);

    expect_value(__wrap_json_object_array_add, obj, jtimewrap);
    expect_value(__wrap_json_object_array_add, val, jtvSec);
    will_return(__wrap_json_object_array_add, __real_json_object_array_add(jtimewrap, jtvSec));

    struct json_object *jtvNsec = __real_json_object_new_int64(timestamp.tv_nsec);
    expect_value(__wrap_json_object_new_int64, i, timestamp.tv_nsec);
    will_return(__wrap_json_object_new_int64, jtvNsec);

    expect_value(__wrap_json_object_array_add, obj, jtimewrap);
    expect_value(__wrap_json_object_array_add, val, jtvNsec);
    will_return(__wrap_json_object_array_add, -1);

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(jobj, NULL, &timestamp);

    assert_null(jtimestamp);

    jtimestamp = json_object_array_get_idx(jobj, idx);

    assert_null(jtimestamp);

    MOCK_FUNC_NEVER(json_object_new_array);
    MOCK_FUNC_NEVER(json_object_new_int64);
    MOCK_FUNC_NEVER(json_object_array_add);
}

void safuTestSafuJsonAddNewTimestampErrInsertNSecArray(void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to insert the nano seconds into a json timestamp for a json array");

    *(struct json_object **)state = __real_json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    const struct timespec testRows[] = {
        {0, 0},         {0, 42},         {0, INT64_MAX},         {0, INT64_MIN},         {0, -12},
        {42, 0},        {42, 42},        {42, INT64_MAX},        {42, INT64_MIN},        {42, -12},
        {INT64_MAX, 0}, {INT64_MAX, 42}, {INT64_MAX, INT64_MAX}, {INT64_MAX, INT64_MIN}, {INT64_MAX, -12},
        {INT64_MIN, 0}, {INT64_MIN, 42}, {INT64_MIN, INT64_MAX}, {INT64_MIN, INT64_MIN}, {INT64_MIN, -12},
        {-12, 0},       {-12, 42},       {-12, INT64_MAX},       {-12, INT64_MIN},       {-12, -12},
    };
    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i, testRows[i]);
    }
}
