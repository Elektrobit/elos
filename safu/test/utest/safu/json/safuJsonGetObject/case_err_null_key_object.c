// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetObject_utest.h"

SETUP(safuTestSafuJsonGetObjectErrNullKeyObject)
TEARDOWN(safuTestSafuJsonGetObjectErrNullKeyObject)

static void _testCase(struct json_object *jobj, size_t idx) {
    PARAM("idx = %lu", idx);

    struct json_object *res = safuJsonGetObject(jobj, NULL, idx);

    assert_null(res);
}

void safuTestSafuJsonGetObjectErrNullKeyObject(void **state) {
    TEST("safuJsonGetObject");
    SHOULD("%s", "fail to get a json object from a json object with a int as key");

    *(struct json_object **)state = json_object_new_object();
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
        safuJsonAddObject(jobj, testRows[i].key, testRows[i].data);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i);
    }
}
