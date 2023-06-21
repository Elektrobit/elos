// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetObject_utest.h"

SETUP(safuTestSafuJsonGetObjectErrObject)
TEARDOWN(safuTestSafuJsonGetObjectErrObject)

static void _testCase(struct json_object *jobj, const char *key) {
    PARAM("key = \"%s\"", key);

    MOCK_FUNC_AFTER_CALL(json_object_object_get, 0);

    expect_value(__wrap_json_object_object_get, obj, jobj);
    expect_value(__wrap_json_object_object_get, key, key);
    will_return(__wrap_json_object_object_get, NULL);

    struct json_object *res = safuJsonGetObject(jobj, key, 0);

    assert_null(res);
}

void safuTestSafuJsonGetObjectErrObject(void **state) {
    TEST("safuJsonGetObject");
    SHOULD("%s", "fail to get a json object from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    struct {
        char *key;
        struct json_object *data;
    } testRows[] = {
        {"fortytwo", json_object_new_int(42)}, {"string", json_object_new_string("JSON String-Object")},
        {"object", json_object_new_object()},  {"array", json_object_new_array()},
        {"", json_object_new_null()},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddObject(jobj, testRows[i].key, testRows[i].data);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key);
    }
}
