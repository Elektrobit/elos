// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectErrStringIndexArray)
TEARDOWN(safuTestSafuJsonAddObjectErrStringIndexArray)

static void _testCase(struct json_object *jarray, const char *key, struct json_object *jadd) {
    PARAM("key = \"%s\"", key);

    int retval = safuJsonAddObject(jarray, key, jadd);
    assert_int_equal(retval, -1);
    assert_int_equal(json_object_array_length(jarray), 0);
}

void safuTestSafuJsonAddObjectErrStringIndexArray(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "fail to add a JSON object to a JSON array with a sting as index key");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct {
        char *name;
        struct json_object *data;
    } testRows[] = {
        {"fortytwo", json_object_new_int(42)}, {"string", json_object_new_string("JSON String-Object")},
        {"object", json_object_new_object()},  {"array", json_object_new_array()},
        {"", json_object_new_null()},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i].name, testRows[i].data);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        json_object_put(testRows[i].data);
    }
}
