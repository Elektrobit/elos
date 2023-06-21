// SPDX-License-Identifier: MIT
#include "safuJsonGetArray_utest.h"

SETUP(safuTestSafuJsonGetArrayErrNoArrayObject)
TEARDOWN(safuTestSafuJsonGetArrayErrNoArrayObject)

static void _testCase(const struct json_object *jobj, const char *name) {
    struct json_object *resultArray = safuJsonGetArray(jobj, name, 0, NULL);

    assert_null(resultArray);
}

void safuTestSafuJsonGetArrayErrNoArrayObject(void **state) {
    TEST("safuJsonGetArray");
    SHOULD("%s", "fail to get a json array from a json object, get a NULL object instead");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    char *testRows[] = {
        "empty",
        "one",
        "short",
        "longer",
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddObject(jobj, testRows[i], json_object_new_int(i));
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
