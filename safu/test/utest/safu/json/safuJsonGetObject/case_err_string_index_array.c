// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetObject_utest.h"

SETUP(safuTestSafuJsonGetObjectErrStringIndexArray)
TEARDOWN(safuTestSafuJsonGetObjectErrStringIndexArray)

static void _testCase(struct json_object *jobj, const char *key) {
    PARAM("key = \"%s\"", key);
    struct json_object *res = safuJsonGetObject(jobj, key, 0);

    assert_null(res);
}

void safuTestSafuJsonGetObjectErrStringIndexArray(void **state) {
    TEST("safuJsonGetObject");
    SHOULD("%s", "fail to get a json object from a json array with a string as key");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        struct json_object *data;
    } testRows[] = {
        {"fortytwo", json_object_new_int(42)}, {"string", json_object_new_string("JSON String-Object")},
        {"object", json_object_new_object()},  {"array", json_object_new_array()},
        {"", json_object_new_null()},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddObject(jobj, NULL, testRows[i].data);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key);
    }
}
