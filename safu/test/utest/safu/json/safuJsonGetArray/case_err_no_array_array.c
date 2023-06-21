// SPDX-License-Identifier: MIT
#include "safuJsonGetArray_utest.h"

SETUP(safuTestSafuJsonGetArrayErrNoArrayArray)
TEARDOWN(safuTestSafuJsonGetArrayErrNoArrayArray)

static void _testCase(const struct json_object *jobj, size_t idx) {
    struct json_object *resultArray = safuJsonGetArray(jobj, NULL, idx, NULL);

    assert_null(resultArray);
}

void safuTestSafuJsonGetArrayErrNoArrayArray(void **state) {
    TEST("safuJsonGetArray");
    SHOULD("%s", "fail to get a json array from a json array, get a NULL object instead");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;

    size_t numTestRows = 5;
    for (size_t i = 0; i < numTestRows; i++) {
        safuJsonAddObject(jobj, NULL, json_object_new_int(i));
    }

    for (size_t i = 0; i < numTestRows; i++) {
        _testCase(jobj, i);
    }
}
