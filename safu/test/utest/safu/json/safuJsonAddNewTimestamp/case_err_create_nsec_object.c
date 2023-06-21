// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

SETUP(safuTestSafuJsonAddNewTimestampErrCreateNSecObject)
TEARDOWN(safuTestSafuJsonAddNewTimestampErrCreateNSecObject)

static void _testCase(struct json_object *jobj, const char *key, struct timespec timestamp) {
    PARAM("key = \"%s\"", key);
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

    expect_value(__wrap_json_object_new_int64, i, timestamp.tv_nsec);
    will_return(__wrap_json_object_new_int64, NULL);

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(jobj, key, &timestamp);

    assert_null(jtimestamp);

    jtimestamp = json_object_object_get(jobj, key);

    assert_null(jtimestamp);

    MOCK_FUNC_NEVER(json_object_new_array);
    MOCK_FUNC_NEVER(json_object_new_int64);
    MOCK_FUNC_NEVER(json_object_array_add);
}

void safuTestSafuJsonAddNewTimestampErrCreateNSecObject(void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to create the json object with the nano seconds for a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    const struct {
        char *key;
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
        _testCase(jobj, testRows[i].key, testRows[i].time);
    }
}
