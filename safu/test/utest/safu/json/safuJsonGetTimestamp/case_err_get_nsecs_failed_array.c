// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

SETUP(safuTestSafuJsonGetTimestampErrGetNSecsFailedArray)
TEARDOWN(safuTestSafuJsonGetTimestampErrGetNSecsFailedArray)

static void _testCase(const struct json_object *jobj, size_t idx, struct timespec val) {
    PARAM("idx = %lu", idx);
    PARAM("val = { tv_sec = %ld, tv_nsec = %ld }", val.tv_sec, val.tv_nsec);
    struct timespec result = {70, 70};

    expect_any(safuJsonGetInt64, jobj);
    expect_value(safuJsonGetInt64, name, NULL);
    expect_value(safuJsonGetInt64, idx, 0);
    expect_any(safuJsonGetInt64, val);
    will_return_success(safuJsonGetInt64, val.tv_sec);

    expect_any(safuJsonGetInt64, jobj);
    expect_value(safuJsonGetInt64, name, NULL);
    expect_value(safuJsonGetInt64, idx, 1);
    expect_any(safuJsonGetInt64, val);
    will_return_error(safuJsonGetInt64);

    int retval = safuJsonGetTimestamp(jobj, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result.tv_sec, 70);
    assert_int_equal(result.tv_nsec, 70);
}

void safuTestSafuJsonGetTimestampErrGetNSecsFailedArray(void **state) {
    TEST("safuJsonGetTimestamp");
    SHOULD("%s", "fail to get the nanosecond of a Timestamp from a json array");

    *(struct json_object **)state = json_object_new_array();
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
        safuJsonAddNewTimestamp(jobj, NULL, &testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i, testRows[i]);
    }
}
