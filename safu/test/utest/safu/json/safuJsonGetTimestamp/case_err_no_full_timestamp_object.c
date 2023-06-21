// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

SETUP(safuTestSafuJsonGetTimestampErrNoFullTimestampObject)
TEARDOWN(safuTestSafuJsonGetTimestampErrNoFullTimestampObject)

static void _testCase(const struct json_object *jobj, const char *name) {
    PARAM("name = \"%s\"", name);
    struct timespec result = {70, 70};
    int retval = safuJsonGetTimestamp(jobj, name, 0, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result.tv_sec, 70);
    assert_int_equal(result.tv_nsec, 70);
}

void safuTestSafuJsonGetTimestampErrNoFullTimestampObject(void **state) {
    TEST("safuJsonGetTimestamp");
    SHOULD("%s", "fail to get a full Timestamp from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    const struct {
        char *name;
        uint64_t time;
    } testRows[] = {
        {"zero,zero", 0}, {"zero,some", 42}, {"zero/max", INT64_MAX}, {"zero|min", INT64_MIN}, {"zero minus", -12},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        struct json_object *array = json_object_new_array();
        safuJsonAddNewInt64(array, NULL, testRows[i].time);
        safuJsonAddObject(jobj, testRows[i].name, array);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name);
    }
}
