// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

SETUP(safuTestSafuJsonAddNewTimestampErrNoTimespecObject)
TEARDOWN(safuTestSafuJsonAddNewTimestampErrNoTimespecObject)

static void _testCase(struct json_object *jobj, const char *key) {
    PARAM("key = \"%s\"", key);

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(jobj, key, NULL);

    assert_null(jtimestamp);

    jtimestamp = json_object_object_get(jobj, key);

    assert_null(jtimestamp);
}

void safuTestSafuJsonAddNewTimestampErrNoTimespecObject(void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to provide a timestamp to add to a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    const char *testRows[] = {
        "zero,zero",     "zero,some",        "zero/max",          "zero|min",        "zero minus",
        "stuff-nothing", "stuff_stuff",      "stuff, Max",        "stuff MINIMUM",   "somthing and somthing negativ",
        "Maximum:0",     "Maximum|fortytwo", "Maximum+Maximum",   "MaximumMinimum",  "Maximum|minus",
        "Mini/zero",     "Mini fortytwo",    "Mini|Maximum",      "Mini*2",          "Mini\\Negativ",
        "negativ!zero",  "negativ fortytwo", "negativ!INT64_MAX", "negativ Minimum", "negativ!negativ",
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
