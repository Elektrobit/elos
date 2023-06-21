// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

SETUP(safuTestSafuJsonGetTimestampSuccessObject)
TEARDOWN(safuTestSafuJsonGetTimestampSuccessObject)

static void _testCase(const struct json_object *jobj, const char *name, struct timespec val) {
    PARAM("name = \"%s\"", name);
    PARAM("val = { tv_sec = %ld, tv_nsec = %ld }", val.tv_sec, val.tv_nsec);
    struct timespec result;

    expect_any(safuJsonGetInt64, jobj);
    expect_value(safuJsonGetInt64, name, NULL);
    expect_value(safuJsonGetInt64, idx, 0);
    expect_any(safuJsonGetInt64, val);
    will_return_success(safuJsonGetInt64, val.tv_sec);

    expect_any(safuJsonGetInt64, jobj);
    expect_value(safuJsonGetInt64, name, NULL);
    expect_value(safuJsonGetInt64, idx, 1);
    expect_any(safuJsonGetInt64, val);
    will_return_success(safuJsonGetInt64, val.tv_nsec);

    int retval = safuJsonGetTimestamp(jobj, name, 0, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result.tv_sec, val.tv_sec);
    assert_int_equal(result.tv_nsec, val.tv_nsec);
}

void safuTestSafuJsonGetTimestampSuccessObject(void **state) {
    TEST("safuJsonGetTimestamp");
    SHOULD("%s", "successfully get a Timestamp from a json object");

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
        _testCase(jobj, testRows[i].name, testRows[i].time);
    }
}
