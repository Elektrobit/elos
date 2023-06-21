// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

SETUP(safuTestSafuJsonGetTimestampErrGetArrayFailedObject)
TEARDOWN(safuTestSafuJsonGetTimestampErrGetArrayFailedObject)

static void _testCase(const struct json_object *jobj, size_t idx) {
    PARAM("idx = %lu", idx);
    struct timespec result = {70, 70};
    int retval = safuJsonGetTimestamp(jobj, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result.tv_sec, 70);
    assert_int_equal(result.tv_nsec, 70);
}

void safuTestSafuJsonGetTimestampErrGetArrayFailedObject(void **state) {
    TEST("safuJsonGetTimestamp");
    SHOULD("%s", "fail to an array with the Timestamp from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    const struct {
        char *name;
        struct timespec time;
    } testRows[] = {
        {"zero,zero", {0, 0}},
        {"zero,some", {0, 42}},
        {"zero/max", {0, INT64_MAX}},
        {"zero|min", {0, INT64_MIN}},
        {"zero minus", {0, -12}},
        {"stuff-nothing", {42, 0}},
        {"stuff_stuff", {42, 42}},
        {"stuff, Max", {42, INT64_MAX}},
        {"stuff MINIMUM", {42, INT64_MIN}},
        {"somthing and somthing negativ", {42, -12}},
        {"Maximum:0", {INT64_MAX, 0}},
        {"Maximum|fortytwo", {INT64_MAX, 42}},
        {"Maximum+Maximum", {INT64_MAX, INT64_MAX}},
        {"MaximumMinimum", {INT64_MAX, INT64_MIN}},
        {"Maximum|minus", {INT64_MAX, -12}},
        {"Mini/zero", {INT64_MIN, 0}},
        {"Mini fortytwo", {INT64_MIN, 42}},
        {"Mini|Maximum", {INT64_MIN, INT64_MAX}},
        {"Mini*2", {INT64_MIN, INT64_MIN}},
        {"Mini\\Negativ", {INT64_MIN, -12}},
        {"negativ!zero", {-12, 0}},
        {"negativ fortytwo", {-12, 42}},
        {"negativ!INT64_MAX", {-12, INT64_MAX}},
        {"negativ Minimum", {-12, INT64_MIN}},
        {"negativ!negativ", {-12, -12}},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewTimestamp(jobj, testRows[i].name, &testRows[i].time);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i);
    }
}
