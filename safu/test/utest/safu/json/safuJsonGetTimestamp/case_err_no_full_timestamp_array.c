// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

SETUP(safuTestSafuJsonGetTimestampErrNoFullTimestampArray)
TEARDOWN(safuTestSafuJsonGetTimestampErrNoFullTimestampArray)

static void _testCase(const struct json_object *jobj, size_t idx) {
    PARAM("idx = %lu", idx);
    struct timespec result = {70, 70};
    int retval = safuJsonGetTimestamp(jobj, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result.tv_sec, 70);
    assert_int_equal(result.tv_nsec, 70);
}

void safuTestSafuJsonGetTimestampErrNoFullTimestampArray(void **state) {
    TEST("safuJsonGetTimestamp");
    SHOULD("%s", "fail to get a full Timestamp from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    int64_t testRows[] = {
        0, 42, INT64_MAX, INT64_MIN, -12,
    };
    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        struct json_object *array = json_object_new_array();
        safuJsonAddNewInt64(array, NULL, testRows[i]);
        safuJsonAddObject(jobj, NULL, array);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i);
    }
}
