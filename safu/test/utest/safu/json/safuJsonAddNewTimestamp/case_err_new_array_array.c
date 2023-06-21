// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

SETUP(safuTestSafuJsonAddNewTimestampErrNewArrayArray)
TEARDOWN(safuTestSafuJsonAddNewTimestampErrNewArrayArray)

static void _testCase(struct json_object *jobj, size_t idx, struct timespec timestamp) {
    PARAM("idx = %lu", idx);
    PARAM("timestamp = { tv_sec = %ld, tv_nsec = %ld }", timestamp.tv_sec, timestamp.tv_nsec);

    MOCK_FUNC_AFTER_CALL(json_object_new_array, 0);
    will_return(__wrap_json_object_new_array, NULL);

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(jobj, NULL, &timestamp);

    assert_null(jtimestamp);

    jtimestamp = json_object_array_get_idx(jobj, idx);

    assert_null(jtimestamp);
}

void safuTestSafuJsonAddNewTimestampErrNewArrayArray(void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to create an array for a timestamp to add to a json array");

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
